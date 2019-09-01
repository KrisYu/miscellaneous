#include <cmath>
#include <iostream>


float pdf(const float &x){
  return 1 / sqrt(2 * M_PI) * exp(-x*x*0.5);
}


float sample(const float &r, const float &minBound, const float & maxBound){

  // create cdf
  int nbins = 32;
  float cdf[nbins+1], dx = (maxBound - minBound) / nbins, sum = 0;
  cdf[0] = 0.f;
  for (size_t n = 1; n < nbins; n++) {
    float x = minBound + dx * n, sum =0;
    float pdf_x = pdf(x) * dx;
    cdf[n] = cdf[n-1] + pdf_x;
    sum += pdf_x;
  }

  cdf[nbins] = 1;

  // sample
  float *ptr = std::lower_bound(cdf, cdf + nbins + 1, r);
  int off = std::max(0, (int)(ptr - cdf - 1));
  float t = (r - cdf[off])/(cdf[off+1] - cdf[off]);
  float x = (off + t) / (float)(nbins);
  return minBound * (1-x) + maxBound * x;
}

int main(int argc, char const *argv[]) {

  float minBound = -5, maxBound = 5;

  int numSims = 100000;
  const int numBins = 100;
  int bins[numBins] = {0};
  for (size_t i = 0; i < numSims; i++) {
    float r = drand48();
    float diff = sample(r, minBound, maxBound);
    int whichBin = (int)(numBins * (diff - minBound) / (maxBound - minBound));
    bins[whichBin]++;
  }

  float sum  = 0 ;
  for (size_t i = 0; i < numBins; i++) {
    std::cout << 5 * (2 * i / (float)(numBins) - 1) << ", " << bins[i]/(float)numSims << '\n';
    sum +=  bins[i]/(float)numSims;
  }

  std::cout << sum << '\n';

  return 0;
}
