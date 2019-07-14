#include <iostream>
#include <utility>

#include "geometry.h"
#include "tgaimage.h"


using namespace std;

const int width = 512;
const int height = 512;

TGAImage image(width, height, TGAImage::RGB);
TGAColor white = TGAColor(255, 255, 255);

struct Bounding_Box {
  float minx, miny, maxx, maxy;
};

class Shape {
public:
  Shape() {}
  virtual bool in_shape(Vec2f p) = 0;

  void draw() {
    for (float x = box.minx; x < box.maxx; x++) {
      for (float y = box.miny; y < box.maxy; y++) {
         if (in_shape(Vec2f(x,y))) image.set(x, y, white);
      }
    }
  }

  Bounding_Box box;
};

class Circle: public Shape {
private:
  float r; // radius
  Vec2f c; // center


public:
  Circle(): Shape() {}
  Circle(float radius, Vec2f center): r(radius), c(center){
    box.minx = c.x - r;
    box.maxx = c.x + r;
    box.miny = c.y - r;
    box.maxy = c.y + r;
  }

  bool in_shape(Vec2f p){
    if ((p.x-c.x)*(p.x-c.x)+(p.y-c.y)*(p.y-c.y)<=r*r)  return true;
    return false;
  }
};

class Rectangle: public Shape {
private:
  float x, y, w, h;

public:
  Rectangle() : Shape (){}
  Rectangle(float ulx, float uly, float width, float height):x(ulx), y(uly), w(width), h(height){
    box.minx = x;
    box.maxx = x+w;
    box.miny = y;
    box.maxy = y+h;
  }

  bool in_shape(Vec2f p){
    if ((p.x >= box.minx && p.x <= box.maxx) && (p.y >= box.miny && p.y <= box.maxy))  return true;
    return false;
  }
};

class Capsule: public Shape {
private:
  Vec2f a, b;
  float r;

public:
  Capsule(): Shape() {}
  Capsule(Vec2f A, Vec2f B, float R): a(A), b(B), r(R){
    box.minx = (a.x < b.x)? a.x-2.f*r : b.x-2.f*r;
    box.maxx = (a.x < b.x)? b.x+2.f*r : a.x+2.f*r;
    box.miny = (a.y < b.y)? a.y-2.f*r : b.y-2.f*r;
    box.maxy = (a.y < b.y)? b.y+2.f*r : b.x+2.f*r;
  }

  bool in_shape(Vec2f p){
    Vec2f AP = p - a;
    Vec2f AB = b - a;
    float t = min(1.0f, max(0.0f, (AP*AB)/(AB*AB)));
    Vec2f projp = a*(1-t)+b*t;
    if ((p.x-projp.x)*(p.x-projp.x)+(p.y-projp.y)*(p.y-projp.y)<=r*r)  return true;
    return false;
  }

};

class Triangle: public Shape{
private:
  Vec2f p0, p1, p2;
public:
  Triangle(): Shape() {}
  Triangle(Vec2f A, Vec2f B, Vec2f C): p0(A), p1(B), p2(C){
    box.minx = fmin(p0.x, fmin(p1.x, p2.x));
    box.maxx = fmax(p0.x, fmax(p1.x, p2.x));
    box.miny = fmin(p0.y, fmin(p1.y, p2.y));
    box.maxy = fmax(p0.y, fmax(p1.y, p2.y));
  }

  bool in_shape(Vec2f p){
    float x0 = p0.x, x1=p1.x, x2=p2.x;
    float y0 = p0.y, y1=p1.y, y2=p2.y;
    float x = p.x, y = p.y;

    bool b0 = (x-x0)*(y1-y0)-(y-y0)*(x1-x0) < 0;
    bool b1 = (x-x1)*(y2-y1)-(y-y1)*(x2-x1) < 0;
    bool b2 = (x-x2)*(y0-y2)-(y-y2)*(x0-x2) < 0;

    if ((b0==b1) && (b1==b2)) return true;
    return false;
  }
};

struct Object{
  vector<Shape*> parts;


  void draw() {
    for (auto part: parts) part->draw();
  }
};


int main(int argc, char const *argv[]) {

  Object human;
  Circle head(20, Vec2f(100,200));
  Rectangle body(60,220,80,90);
  Capsule larm(Vec2f(50,230), Vec2f(40,280), 10);
  Capsule rarm(Vec2f(150,230), Vec2f(170,280), 10);
  Capsule lleg(Vec2f(70,320), Vec2f(70,380), 10);
  Capsule rleg(Vec2f(130,320), Vec2f(130,380), 10);

  human.parts.push_back(&head);
  human.parts.push_back(&body);
  human.parts.push_back(&larm);
  human.parts.push_back(&rarm);
  human.parts.push_back(&lleg);
  human.parts.push_back(&rleg);

  human.draw();


  Object house;
  Rectangle wall(220,150, 240, 260);
  Triangle roof(Vec2f(200,150), Vec2f(480,150), Vec2f(340,100));
  Rectangle chimney(400,100,20,50);

  house.parts.push_back(&wall);
  house.parts.push_back(&roof);
  house.parts.push_back(&chimney);

  house.draw();

  image.write_tga_file("output.tga");
  return 0;
}
