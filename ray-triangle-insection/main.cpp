#include <iostream>

#include "geometry.h"
#include "tgaimage.h"

using namespace std;

const float epsilon = 0.001;


bool ray_triangle_intersect(const Vec3f A, const Vec3f B, const Vec3f p0, const Vec3f p1, const Vec3f p2){

  // 1. find the p point
  Vec3f n = cross(p1 - p0, p2 - p0);
  float d = -n*p1;

  Vec3f AB = B - A;
  if (fabsf(n*AB) < epsilon) return false;

  float t = (-d - n*A )/(n * AB);
  // t < 0 means opposite direction of AB
  if (t < 0 ) return false;
  Vec3f p = A + AB*t;

  if (cross(p1-p0, p-p0) * n < 0 ) return false;
  if (cross(p2-p1, p-p1) * n < 0 ) return false;
  if (cross(p0-p2,p-p2) * n < 0 ) return false;
  return true;
}

// AB: ray, p0p1p2: triangle
bool bary_centric_coordinate(const Vec3f A, const Vec3f B, const Vec3f p0, const Vec3f p1, const Vec3f p2, float &u, float &v){

  // 1. find the p point
  Vec3f n = cross(p1 - p0, p2 - p0);
  float area = n.norm();
  float d = -n*p1;

  Vec3f AB = B - A;
  if (fabsf(n*AB) < epsilon) return false;

  float t = (-d - n*A )/(n * AB);
  if (t < 0 ) return false;
  Vec3f p = A + AB*t;

  // 2. find u,v
  Vec3f uvector, vvector;
  if (cross(p2-p1, p-p1) * n < 0 ) return false;
  if ((vvector = cross(p1-p0,p-p0)) * n < 0) return false;
  if ((uvector = cross(p0-p2,p-p2)) * n < 0) return false;

  u = uvector.norm()/area;
  v = vvector.norm()/area;

  return true;
}

// OD: ray, p0p1p2: triangle
bool ray_triangle_intersect_mt(const Vec3f O, const Vec3f D, const Vec3f p0, const Vec3f p1, const Vec3f p2, float &t, float &u, float &v){
  Vec3f e1 = p1-p0;
  Vec3f e2 = p2-p0;
  Vec3f pvec = cross(D, e2);
  float det = e1*pvec;
  if (det < epsilon) return false;

  Vec3f tvec = O - p0;
  u = tvec*pvec*(1./det);
  if (u < 0 || u > 1) return false;

  Vec3f qvec = cross(tvec, e1);
  v = D*qvec*(1./det);
  if (v < 0 || u + v > 1) return false;

  t = e2*qvec*(1./det);
  return t > epsilon;
}

int main(int argc, char const *argv[]) {

  Vec3f A(-200,-200,-300);
  Vec3f p0(50,300,0);
  Vec3f p1(340,270,0);
  Vec3f p2(200,30,0);

  const int width = 400;
  const int height = 400;

  TGAImage image(width, height, TGAImage::RGB);
  float u, v, t;

  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      Vec3f B(x,y,0);
      if (ray_triangle_intersect_mt(A, B-A, p0, p1, p2, u, v, t)) {
        unsigned char red = (1-u-v)*0 + u*255 + v*0;
        unsigned char green = (1-u-v)*0 + u*0 + v*255;
        unsigned char blue = (1-u-v)*255 + u*0 + v*0;
        TGAColor color(red,green,blue);
        image.set(x,y,color);
      }
    }
  }

  image.write_tga_file("output.tga");
  return 0;
}
