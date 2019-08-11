#ifndef TEXTUREH
#define TEXTUREH

#include "valuenoise.h"

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

class noise_texture: public texture{
public:
  noise_texture() {}
  noise_texture(float f, float a): frequency(f), amplitude(a) {}
  vec3 value(float u, float v, const vec3& p) const{
    float g = noise.eval(p * frequency) * amplitude;
    g = g - (int)g;
    return vec3(g, g, g);
  }

  valuenoise noise;
  float frequency, amplitude;
};



#endif
