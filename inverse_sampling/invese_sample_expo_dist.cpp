#include <iostream>
#include <cmath>

int main(int argc, char const *argv[]) {

  int N = 100000;
  const int numBins = 1000;
  int bins[numBins] = {0};
  float cdf[numBins] = {0.0};
  const float maxBound = 5.0;
  const float minBound = 0.0;

  for (size_t i = 0; i < N; i++) {
    float e = -logf(drand48());
    if (e >= maxBound) e = maxBound;
    int whichBin = (int)(numBins * (e - minBound) / (maxBound - minBound));
    bins[whichBin]++;
  }

  // pdf - draw histgram numBins graph
  for (size_t i = 0; i < numBins; i++) {
    std::cout << 5.f * i /(float)numBins  << ", " << bins[i]/float(N) << std::endl;
  }

  // cdf
  cdf[0] = 0;
  for (size_t i = 1; i < numBins-1; i++) {
    cdf[i] = bins[i]/float(N) + (i == 0 ? 0 : cdf[i-1]);
  }
  cdf[numBins - 1] = 1;

  for (size_t i = 0; i < numBins; i++) {
    std::cout << 5.f * i /(float)numBins << ", " << cdf[i] << std::endl;
  }



  return 0;
}
