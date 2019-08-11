#ifndef VALUENOISEH
#define VALUENOISEH

#include <cmath>

#include "vec3.h"

float lerp(float a,float b, float t){
  return  a*(1-t) + b*t;
}

inline float smoothstep(const float &t) { return t * t * (3 - 2 * t); }

class valuenoise {
private:
  static const int tableSize = 256;
  static const int tableSizeMask = tableSize - 1;
  float r[tableSize];
  int pTable[tableSize * 2];
  int hash(const int &x, const int &y, const int &z) const{
    return pTable[pTable[pTable[x]+y]+z];
  }
public:
  valuenoise (){
    for (size_t i = 0; i < tableSize; i++) {
      r[i] = drand48();
      pTable[i] = i;
    }

    // shuffle
    for (size_t i = tableSize - 1; i > 0; i--) {
      int k = int(drand48() * (i+1));
      std::swap(pTable[k], pTable[i]);
      pTable[i+tableSize] = pTable[i];
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

    float sx = smoothstep(tx);
    float sy = smoothstep(ty);
    float sz = smoothstep(tz);

    const float &c000 = r[hash(xi0,yi0,zi0)];
    const float &c100 = r[hash(xi1,yi0,zi0)];
    const float &c010 = r[hash(xi0,yi1,zi0)];
    const float &c110 = r[hash(xi1,yi1,zi0)];

    const float &c001 = r[hash(xi0,yi0,zi1)];
    const float &c101 = r[hash(xi1,yi0,zi1)];
    const float &c011 = r[hash(xi0,yi1,zi1)];
    const float &c111 = r[hash(xi1,yi1,zi1)];

    float a = lerp(c000, c100, sx);
    float b = lerp(c010, c110, sx);
    float c = lerp(c001, c101, sx);
    float d = lerp(c011, c111, sx);

    float e = lerp(a, b, sy);
    float f = lerp(c, d, sy);

    return lerp(e, f, sz);
  }

};

#endif
