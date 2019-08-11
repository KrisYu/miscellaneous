#include "vec3.h"

int main(){
	int nx = 200;
	int ny = 100;
  
  vec3 rockColors[3] = {
    vec3(0.4078, 0.4078, 0.3764),
    vec3(0.7606, 0.6274, 0.6313),
    vec3(0.8980, 0.9372, 0.9725)
  };
	
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	
	for(int j = ny - 1; j >= 0; j--){
		for (int i = 0; i < nx; i++){
      vec3 col = rockColors[int(3*drand48())];
			
      int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	
}
