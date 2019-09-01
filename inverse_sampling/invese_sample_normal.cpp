#include <cmath>
#include <iostream>


float pdf(const float &x){
  return 1 / sqrt(2 * M_PI) * exp(-x*x*0.5);
}


int main(int argc, char const *argv[]) {

  // generate needed cdf
  int nbins = 32;
  float minBound = -5, maxBound = 5;
  float cdf[nbins+1], dx = (maxBound - minBound) / nbins, sum = 0;
  cdf[0] = 0.f;
  for (size_t n = 1; n < nbins; n++) {
    float x = minBound + dx * n, sum =0;
    float pdf_x = pdf(x) * dx;
    cdf[n] = cdf[n-1] + pdf_x;
    sum += pdf_x;
  }

  cdf[nbins] = 1;

  // inverse sample step

  srand48(13);
  float r = drand48();
  // https://en.cppreference.com/w/cpp/algorithm/lower_bound
  // get the r range
  float *ptr = std::lower_bound(cdf, cdf + nbins + 1, r);
  int off = (int)(ptr - cdf - 1);
  std::cout << "r " << r << "\noffset " << off << '\n';
  std::cout << "cdf[off] " << cdf[off] << " cdf[off+1] " << cdf[off+1] << '\n';

  float t = (r - cdf[off])/(cdf[off+1] - cdf[off]);
  std::cout << "t " << t << '\n';
  float x = (off + t) / (float)(nbins);
  x = minBound * (1-x) + maxBound * x;
  std::cout << "x " << x << '\n';

  return 0;
}
