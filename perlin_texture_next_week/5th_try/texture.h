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
  perlin_texture(float sc): scale(sc) {}
  vec3 value(float u, float v, const vec3& p) const{
    return vec3(1,1,1) * noise.noise(scale * p);
  }
  perlin noise;
  float scale;
};



#endif
