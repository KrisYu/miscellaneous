#include <vector>
#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "geometry.h"

using std::vector;
using std::swap;
using std::sqrt; using std::cos; using std::sin;

const TGAColor white = TGAColor(255, 255, 255, 255);
const int width  = 300;
const int height = 300;


struct Face
{
  int i0, i1, i2;
  Face(int ii0, int ii1, int ii2): i0(ii0), i1(ii1), i2(ii2) {};
};

// draw line
void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(p0.x-p1.x)<std::abs(p0.y-p1.y)) {
        swap(p0.x, p0.y);
        swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x>p1.x) {
        swap(p0, p1);
    }

    for (int x=p0.x; x<=p1.x; x++) {
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        int y = p0.y*(1.-t) + p1.y*t;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

// draw filled triangle
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
  // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!)
  if (t0.y>t1.y) swap(t0, t1);
  if (t0.y>t2.y) swap(t0, t2);
  if (t1.y>t2.y) swap(t1, t2);
  int total_height = t2.y-t0.y;
  for (int y=t0.y; y<=t1.y; y++) {
    int segment_height = t1.y-t0.y+1;
    float alpha = (float)(y-t0.y)/total_height;
    float beta  = (float)(y-t0.y)/segment_height; // be careful with divisions by zero
    Vec2i A = t0 + (t2-t0)*alpha;
    Vec2i B = t0 + (t1-t0)*beta;
    if (A.x>B.x) swap(A, B);
    for (int j=A.x; j<=B.x; j++) {
      image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
    }
  }
  for (int y=t1.y; y<=t2.y; y++) {
    int segment_height =  t2.y-t1.y+1;
    float alpha = (float)(y-t0.y)/total_height;
    float beta  = (float)(y-t1.y)/segment_height; // be careful with divisions by zero
    Vec2i A = t0 + (t2-t0)*alpha;
    Vec2i B = t1 + (t2-t1)*beta;
    if (A.x>B.x) swap(A, B);
    for (int j=A.x; j<=B.x; j++) {
      image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
    }
  }
}

void outline(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
  line(t0, t1, image, color);
  line(t1, t2, image, color);
  line(t2, t0, image, color);
}

// generate sphere as explained
void sphere(float r, int n, std::vector<Vec3f>& vertexes, std::vector<Face>& faces) {

  for (int d = 0; d <= n; d++) {
    float y = 2. * r * d / n - r;
    for (int i = 0; i < n; i++) {
      float alpha = 2. * M_PI * i / n;
      float rprime = sqrt(r * r - y * y);
      float x = rprime * cos(alpha);
      float z = rprime * sin(alpha);
      vertexes.push_back(Vec3f(x, y, z));
    }
  }

  for (int i = 0; i < n; i++) {
    int start = i * n;
    for (int j = 0; j < n-1; j++) {
      int a = start + j;
      int b = a + 1;
      int c = a + n;
      int d = c + 1;

      Face abc(a, b, c);
      Face bdc(b, d, c);

      faces.push_back(abc);
      faces.push_back(bdc);
    }
  }




}

int main(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);
    Vec3f light_dir(0,0,-1);

    std::vector<Vec3f> vertexes;
    std::vector<Face> faces;

    sphere(0.9, 50, vertexes, faces);

    for (int i = 0; i < faces.size(); i++) {
      Vec3f world_coords[3];
      Vec2i screen_coords[3];

      Face face = faces[i];

      world_coords[0] = vertexes[face.i0];
      world_coords[1] = vertexes[face.i1];
      world_coords[2] = vertexes[face.i2];

      for (int j = 0; j < 3; j++) {
        Vec3f v = world_coords[j];
        screen_coords[j] = Vec2i((v.x+1.)*width/2., (v.y+1.)*height/2.);
      }

      Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
      n.normalize();
      float intensity = n*light_dir;
      if (intensity > 0) {
        triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
      }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}
