#ifndef TEXTUREH
#define TEXTUREH

#include "perlin.h"

class texture {
public:
  virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture: public texture{
public:
  constant_texture(vec3 c): color(c) {}
  vec3 value(float u, float v, const vec3& p) const{
    return color;
  }
  vec3 color;
};


class perlin_texture: public texture{
public:
  perlin_texture() {}
  perlin_texture(float f): frequency(f) {}
  vec3 value(float u, float v, const vec3& p) const{
    return vec3(1,1,1) * 0.5 *(1 + sin(5 *p.z() + 5 * noise.turb(p*frequency)));
  }
  perlin noise;
  float frequency;

};



#endif
