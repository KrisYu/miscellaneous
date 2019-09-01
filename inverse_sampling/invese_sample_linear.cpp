#include <iostream>
#include <cmath>

int main(int argc, char const *argv[]) {
  int N = 1000000;
  const int bar = 200;
  int nums[bar] = {0};
  float cdf[bar] = {0.0};

  for (size_t i = 0; i < N; i++) {
    float e = sqrt(4*drand48());
    int slot = 100 * e; // 0 ~ 200
    nums[slot]++;
  }

  // pdf - draw histgram bar graph
  for (size_t i = 0; i < bar; i++) {
    std::cout << i /(float)100 << ", " << nums[i]/float(N) << std::endl;
  }

  // cdf
  float sum = 0;
  for (size_t i = 0; i < bar; i++) {
    cdf[i] = nums[i]/float(N) + (i == 0 ? 0 : cdf[i-1]);
    std::cout << i /(float)100 << ", " << cdf[i] << std::endl;

  }



  return 0;
}
