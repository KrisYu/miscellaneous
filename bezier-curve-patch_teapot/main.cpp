
#include <iostream>
#include <vector>

#include "geometry.h"
#include "tgaimage.h"
#include "teaspoondata.h"


using namespace std;


Vec3f bezier(const Vec3f *cps, const float &t){

  float b0 = (1-t)*(1-t)*(1-t);
  float b1 = 3*t*(1-t)*(1-t);
  float b2 = 3*t*t*(1-t);
  float b3 = t*t*t;


  return cps[0]*b0+cps[1]*b1+cps[2]*b2+cps[3]*b3;
}

Vec3f patch(const Vec3f *cps, const float &u, const float &v){
  Vec3f pu[4];
  for (int i = 0; i < 4; i++) pu[i] = bezier(cps+4*i, u);
  return bezier(pu, v);
}


Vec3f dUBezier(const Vec3f *cps, const float &u, const float &v){
  Vec3f P[4];
  Vec3f vCurve[4];

  for (size_t i = 0; i < 4; i++) {
    P[0] = cps[i];
    P[1] = cps[4+i];
    P[2] = cps[8+i];
    P[3] = cps[12+i];
    vCurve[i] = bezier(P, v);
  }

  return vCurve[0]*-3*(1-u)*(1-u) +
  vCurve[1]*(3*(1-u)*(1-u)-6*(1-u)*u) +
  vCurve[2]*(6*u*(1-u)-3*u*u)+
  vCurve[3]*3*u*u;
}

Vec3f dVBezier(const Vec3f *cps, const float &u, const float &v){

  Vec3f uCurve[4];
  for (size_t i = 0; i < 4; i++) uCurve[i] = bezier(cps+4*i,u);

  return uCurve[0]*-3*(1-v)*(1-v) +
  uCurve[1]*(3*(1-v)*(1-v)-6*(1-v)*v) +
  uCurve[2]*(6*v*(1-v)-3*v*v)+
  uCurve[3]*3*v*v;
}


int main(int argc, char const *argv[]) {

  const int divs = 8;


  vector<Vec3f> points;
  vector<Vec3f> normals;
  vector<vector<unsigned>> faces;

  for (int k = 0; k < kNumPatches; k++) {

    Vec3f cps[16];
    // 16 control point for a patch
    for (int i = 0; i < 16; i++) {
      int idx = kPatches[k][i];
      cps[i] = kVertices[idx-1];
    }

    for (int i = 0; i <= divs; i++) {
      for (int j = 0; j <= divs ; j++) {
        float u = float(i)/divs;
        float v = float(j)/divs;
        Vec3f point = patch(cps,u,v);
        points.push_back(point);

        Vec3f du = dUBezier(cps, u, v);
        Vec3f dv = dVBezier(cps, u, v);
        Vec3f normal = cross(du, dv).normalize();
        normals.push_back(normal);
      }
    }
  }


  for (size_t k = 0; k < kNumPatches; k++) {
    for (size_t i = 0; i < divs; i++) {
      unsigned start = k *(divs+1)* (divs+1);
      for (size_t j = 0; j < divs; j++) {
        unsigned a = start + i * (divs+1) + j;
        unsigned b = a + 1;
        unsigned c = a + (divs+1) ;
        unsigned d = c + 1;

        vector<unsigned> tri1, tri2;
        tri1.push_back(c);
        tri1.push_back(a);
        tri1.push_back(b);

        tri2.push_back(d);
        tri2.push_back(c);
        tri2.push_back(b);

        faces.push_back(tri1);
        faces.push_back(tri2);

      }
    }
  }

  // for (size_t k = 0; k < kTeacupNumPatches; k++) {
  //   unsigned start = k *(divs+1)*(divs+1) ;
  //   if (k > 0) start -= (divs+1);
  //
  // }

  // points.size = (divs+1)*(divs+1)*kTeapotNumPatches;
  // cout << points.size() << endl;

  for (int i = 0; i < points.size(); i++) {
    cout << "v " << points[i] << endl;
  }

  for (size_t i = 0; i < normals.size(); i++) {
    cout << "vn " << normals[i] << endl;
  }

  for (size_t i = 0; i < faces.size(); i++) {
    cout << "f ";

    for (size_t j = 0; j < 3; j++) {
      unsigned f = faces[i][j] + 1;
      cout << f << "/" << "1" << "/" << f << " ";
    }

    cout << endl;
  }


  return 0;
}
