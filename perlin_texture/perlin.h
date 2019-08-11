#ifndef PERLINH
#define PERLINH

#include <cmath>
#include <random>

#include "vec3.h"

template<typename T>
inline T lerp(const T &lo, const T &hi, float t)
{ return lo * (1 - t) + hi * t; }

inline float smoothstep(const float &t) { return t * t * (3 - 2 * t); }

class perlin {
private:
  static const unsigned tableSize = 256;
  static const unsigned tableSizeMask = tableSize - 1;
  vec3 gradients[tableSize];
  unsigned pTable[tableSize * 2];
  int hash(const int &x, const int &y, const int &z) const{
    return pTable[pTable[pTable[x]+y]+z];
  }
public:
  perlin (){
    unsigned seed = 2016;
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distrFloat;
    auto randFloat = bind(distrFloat, gen);

    for (unsigned i = 0; i < tableSize; i++) {
      gradients[i] = unit_vector(vec3(2*randFloat()-1,2*randFloat()-1,2*randFloat()-1));
      pTable[i] = i;
    }

    // shuffle
    std::uniform_int_distribution<int> distUInt;
    auto randUInt = bind(distUInt, gen);
    for (size_t k = 0; k < tableSize; k++) {
      unsigned i  = randUInt() & tableSizeMask;
      std::swap(pTable[k], pTable[i]);
      pTable[k+tableSizeMask] = pTable[k];
    }
  }


  // float floor ( float arg );
  float eval(vec3 p) const {
    int xi0 = ((int)std::floor(p.x())) & tableSizeMask;
    int yi0 = ((int)std::floor(p.y())) & tableSizeMask;
    int zi0 = ((int)std::floor(p.z())) & tableSizeMask;

    int xi1 = (xi0 + 1) & tableSizeMask;
    int yi1 = (yi0 + 1) & tableSizeMask;
    int zi1 = (zi0 + 1) & tableSizeMask;

    float tx = p.x() - ((int)std::floor(p.x()));
    float ty = p.y() - ((int)std::floor(p.y()));
    float tz = p.z() - ((int)std::floor(p.z()));

    float u = smoothstep(tx);
    float v = smoothstep(ty);
    float w = smoothstep(tz);

    const vec3 &c000 = gradients[hash(xi0,yi0,zi0)];
    const vec3 &c100 = gradients[hash(xi1,yi0,zi0)];
    const vec3 &c010 = gradients[hash(xi0,yi1,zi0)];
    const vec3 &c110 = gradients[hash(xi1,yi1,zi0)];

    const vec3 &c001 = gradients[hash(xi0,yi0,zi1)];
    const vec3 &c101 = gradients[hash(xi1,yi0,zi1)];
    const vec3 &c011 = gradients[hash(xi0,yi1,zi1)];
    const vec3 &c111 = gradients[hash(xi1,yi1,zi1)];

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    vec3 p000 = vec3(x0,y0,z0);
    vec3 p100 = vec3(x1,y0,z0);
    vec3 p010 = vec3(x0,y1,z0);
    vec3 p110 = vec3(x1,y1,z0);

    vec3 p001 = vec3(x0,y0,z1);
    vec3 p101 = vec3(x1,y0,z1);
    vec3 p011 = vec3(x0,y1,z1);
    vec3 p111 = vec3(x1,y1,z1);

    float a = lerp(dot(c000, p000), dot(c100, p100), u);
    float b = lerp(dot(c010, p010), dot(c110, p110), u);
    float c = lerp(dot(c001, p001), dot(c101, p101), u);
    float d = lerp(dot(c011, p011), dot(c111, p111), u);

    float e = lerp(a, b, v);
    float f = lerp(c, d, v);

    return lerp(e, f, w);
  }

  float turb(const vec3& p, int depth = 7) const{
    float accum = 0;
    vec3 temp_p = p;
    float weight = 1.0;
    for (size_t i = 0; i < depth; i++) {
      accum += weight * eval(temp_p);
      weight *= 0.5;
      temp_p *= 2;
    }
    return fabs(accum);
  }

};

#endif
