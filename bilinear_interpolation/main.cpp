#include <iostream>
#include <vector>
#include <fstream>

#include "vec3.h"

template<typename T>
inline T lerp(const T &lo, const T &hi, const float &t)
{ return lo * (1 - t) + hi * t; }


int main(int argc, char const *argv[]) {

  const int width = 500;
  const int height = 500;
  const int gridx = 10;
  const int gridy = 10;

  vec3 grid2d[gridx*gridy];

  for (size_t i = 0; i < gridx; i++) {
    for (size_t j = 0; j < gridy; j++) {
      grid2d[i + j * gridx] = vec3(drand48(), drand48(), drand48());
    }
  }

  std::ofstream grid_file("grid.ppm");

	grid_file << "P3\n" << width << " " << height << "\n255\n";

  int grid_width = width/gridx;
  int grid_height = height/gridy;
  for (size_t i = 0; i < width ; i++) {
    for (size_t j = 0; j < height; j++) {
      int i_idx = i / grid_width;
      int j_idx = j / grid_height;

      vec3 col = grid2d[i_idx + j_idx * gridy];
      int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			grid_file << ir << " " << ig << " " << ib << "\n";
    }
  }

  std::ofstream inter_file("interploation.ppm");
  inter_file << "P3\n" << width << " " << height << "\n255\n";

  for (size_t i = 0; i < width; i++) {
    for (size_t j = 0; j < height; j++) {
      // make sure gxi and gyi in range
      float gx = i / float(width) * (gridx-1);
      float gy = j / float(height) * (gridy-1);

      int gxi = int(gx);
      int gyi = int(gy);

      vec3 c00 = grid2d[gxi + gyi*gridy];
      vec3 c10 = grid2d[gxi+1 + gyi*gridy];
      vec3 c01 = grid2d[gxi + (gyi+1)*gridy];
      vec3 c11 = grid2d[gxi+1 + (gyi+1)*gridy];

      vec3 nx0 = lerp(c00, c10, gx - gxi);
      vec3 nx1 = lerp(c01, c11, gx - gxi);
      vec3 col = lerp(nx0, nx1, gy - gyi);

      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);
      inter_file << ir << " " << ig << " " << ib << "\n";
    }
  }

  return 0;
}
