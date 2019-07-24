#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <cmath>

#include "vec3.h"

vec3 random_in_unit_sphere(){
  vec3 p;
   do {
     p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
   } while(dot(p,p) >= 1.0); // can be improved
  return p;
}


//vec3 random_in_unit_sphere(){
//  vec3 p = 2*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
//  p.make_unit_vector();
//  return drand48()*p;
//}

//const double pi = std::acos(-1);
//
//vec3 random_in_unit_sphere(){
//  float phi = drand48() * 2 * pi;
//  float theta = drand48() * 2 * pi;
//  float r = drand48();
//
//
//  return vec3(r * sin(phi)*sin(theta),r* sin(phi)*cos(theta), r* cos(phi));
//}
//
//

//vec3 random_in_unit_sphere(){
//  float u = cbrt(drand48());
//  vec3 p = 2*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
//  p.make_unit_vector();
//  return u*p;
//}


int main(int argc, char const *argv[]) {
  int i = 0;
   int n = 100000000;
//  int n  = 1000;
  std::vector<vec3> points;
  auto timeStart = std::chrono::high_resolution_clock::now();

  while (i < n) {
    vec3 p = random_in_unit_sphere();
    i++;
    points.push_back(p);
  }
  auto timeEnd = std::chrono::high_resolution_clock::now();
	auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
  std::cerr << "\rDone:" << std::setprecision(2) << passedTime / 1000 << "(sec)\n" << std::endl;

//   std::cout << "x, y, z" << std::endl;
//   for(auto point: points){
//     std::cout << point.e[0] << ", " << point.e[1] << ", " << point.e[2] << std::endl;
//   }

  return 0;
}
