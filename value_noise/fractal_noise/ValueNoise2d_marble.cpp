#include <random>
#include <functional>
#include <iostream>
#include <cmath>

using namespace std;

template <typename T>
class Vec2 {
public:
  Vec2(): x(T(0)), y(T(0)) {}
  Vec2(T xx, T yy): x(xx), y(yy) {}
  Vec2 operator * (const T &r) const {return Vec2(x * r, y * r); }
  T x,y;
};

typedef Vec2<float> Vec2f;

template <typename T = float>
inline T lerp(const T &lo, const T &hi, const T &t)
{ return lo * (1-t) + hi * t; }


inline float smoothstep(const float &t) { return t * t * (3 - 2 * t); }

class ValueNoise {
public:
  ValueNoise (unsigned seed = 2016){
    mt19937 gen(seed);
    uniform_real_distribution<float> distrFloat;
    auto randFloat = bind(distrFloat, gen);

    for (unsigned k = 0; k < kMaxTableSize; k++) {
      r[k] = randFloat();
      pTable[k] = k;
    }

    // shuffle values of the permutation pTable
    uniform_int_distribution<int> distUInt;
    auto randUInt = bind(distUInt, gen);
    for (size_t k = 0; k < kMaxTableSize; k++) {
      unsigned i  = randUInt() & kMaxTableSizeMask;
      std::swap(pTable[k], pTable[i]);
      pTable[k+kMaxTableSize] = pTable[k];
    }
  }

  float eval(Vec2f p) const
  {
    int xi = std::floor(p.x);
    int yi = std::floor(p.y);

    float tx = p.x - xi;
    float ty = p.y - yi;

    int rx0 = xi & kMaxTableSizeMask;
    int rx1 = (rx0 + 1) & kMaxTableSizeMask;
    int ry0 = yi & kMaxTableSizeMask;
    int ry1 = (ry0 + 1) & kMaxTableSizeMask;


    // four corner (rx0, ry0), (rx1, ry0), (rx0, ry1), (rx1, ry1)
    const float & c00 = r[pTable[pTable[rx0]]+ ry0];
    const float & c10 = r[pTable[pTable[rx1]]+ ry0];
    const float & c01 = r[pTable[pTable[rx0]]+ ry1];
    const float & c11 = r[pTable[pTable[rx1]]+ ry1];

    float sx = smoothstep(tx);
    float sy = smoothstep(ty);

    //
    float nx0 = lerp(c00, c10, sx);
    float nx1 = lerp(c01, c11, sx);

    return lerp(nx0, nx1, sy);
  }

  static const unsigned kMaxTableSize = 256;
  static const unsigned kMaxTableSizeMask = kMaxTableSize - 1;
  float r[kMaxTableSize];
  unsigned pTable[kMaxTableSize*2];
};

float fractalNoise(Vec2f x){
  ValueNoise noise;
  
  float noiseSum = 0;
  static const unsigned numLayers = 5;
  
  float frequencyMult = 1.8;
  float amplitudeMult = 0.35;
  float frequency = 1.0;
  float amplitude = 1.0;
  
  for (unsigned i = 0; i < numLayers; i++) {
    noiseSum += noise.eval(x * frequency) * amplitude;
    frequency *= frequencyMult;
    amplitude *= amplitudeMult;
  }
  
  return noiseSum ;
}

int main(int argc, char const *argv[]) {

  int nx = 512;
  int ny = 512;


  int ir,ig,ib;
  float frequency = 0.02;
  
  float value[nx*ny];
  float maxValue = 0.0;

  cout << "P3\n" << nx << " " << ny << "\n255\n";

  for (unsigned j = 0; j < ny; j++) {
    for (unsigned i = 0; i < nx; i++) {
      float noiseValue = fractalNoise(Vec2f(i,j)*frequency);
      // we "displace" the value i used in the sin() expression by noiseValue * 100
      value[j*nx +i] = (sin((i + noiseValue * 100) * 2 * M_PI / 200.f) + 1) / 2.f;
      ir = ig = ib = int(255.99 * value[j*nx +i]);
      cout << ir << " " << ig << " " << ib << "\n";
      
    }
  }
  
  
  

  return 0;
}
