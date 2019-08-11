#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>

template<typename T = float>
inline T lerp(const T &lo, const T &hi, const T &t)
{ return lo * (1 - t) + hi * t; }


float cosineRemap(const float &a, const float &b, const float &t){
  assert( t >= 0 && t <= 1);
  float tRemapCosine = (1 - cos(t * M_PI)) * t * 0.5;
  return lerp(a, b, tRemapCosine);
}

float smoothstepRemap(const float &a, const float &b, const float &t){
  assert( t >= 0 && t <= 1);
  float tRemapsmoothstep = t * t * ( 3 - 2 * t);
  return lerp(a, b, tRemapsmoothstep);
}

class ValueNoise1D {
public:
  ValueNoise1D(unsigned seed = 2016){
    srand48(seed);
    for (unsigned i = 0; i < kMaxVertices; i++) {
      r[i] = drand48();
    }
  }
  
  // Evaluate the noise function at position x
  float eval(const float &x){
    int xi = (int)x - (x < 0 && x != (int)x);
    int xMin = xi % kMaxVerticesMask;
    float t = x - xi;
    int xMax = (xMin+1) % kMaxVerticesMask;
    
    return smoothstepRemap(r[xMin], r[xMax], t);
  }
  
  static const unsigned kMaxVertices = 256;
  static const unsigned kMaxVerticesMask = kMaxVertices - 1;
  float r[kMaxVertices];
  
};

float fractalnoise(float x){
  
  ValueNoise1D valueNoise1D;

  
  float noiseSum = 0;
  static const unsigned numLayers = 5;
  
  float amplitude = 1;
  float frequency = 1;
  
  for (unsigned i = 0; i < numLayers; i++) {
    noiseSum += valueNoise1D.eval(x * frequency) * amplitude;
    amplitude *= 0.5;
    frequency *= 2;
  }
  return noiseSum;
}

int main(int argc, char const *argv[]) {
  for (int i = -2000; i <= 2000; i++) {
    float x = 0.01 * i;
    std::cout << x << ", " << fractalnoise(x) << std::endl;
  }
  
  return 0;
}
