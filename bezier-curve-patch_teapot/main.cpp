
#include <iostream>
#include <vector>

#include "geometry.h"
#include "tgaimage.h"
#include "teapotdata.h"


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


int main(int argc, char const *argv[]) {

  const int divs = 8;


  vector<Vec3f> points;
  vector<vector<unsigned>> face;

  for (int k = 0; k < kTeapotNumPatches; k++) {

    Vec3f cps[16];
    // 16 control point for a patch
    for (int i = 0; i < 16; i++) {
      int idx = teapotPatches[k][i];
      cps[i] = teapotVertices[idx-1];
    }

    for (int i = 0; i <= divs; i++) {
      for (int j = 0; j <= divs ; j++) {
        float u = float(i)/divs;
        float v = float(j)/divs;
        points.push_back(patch(cps,u,v));
      }
    }
  }

  // points.size = (divs+1)*(divs+1)*kTeapotNumPatches;
  // cout << points.size() << endl;

  for (int i = 0; i < points.size(); i++) {
    cout << points[i] << endl;
  }

  return 0;
}
