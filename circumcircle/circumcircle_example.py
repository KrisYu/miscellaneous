import math
import random
from PIL import Image, ImageDraw


EPSION = 1e-5

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return tuple(self) == tuple(other)


    def __lt__(self, other):
        # by default we compare with x value
        return self.x <= other.x

    def __hash__(self):
        return hash(tuple(self))

    def __iter__(self):
        return (i for i in (self.x, self.y))

    def __repr__(self):
        class_name = type(self).__name__
        return '{}({!r}, {!r})'.format(class_name, *self)

class Triangle:
    """docstring for Triangle."""

    def __init__(self, v0, v1, v2):
        v0, v1, v2 = sorted([v0, v1, v2])
        self.v0 = v0
        self.v1 = v1
        self.v2 = v2


    def __eq__(self, other):
        return tuple(self) == tuple(other)

    def __hash__(self):
        return hash(tuple(self))

    @classmethod
    def from_vertex_and_edge(cls, v, e):
        return cls(v, e.v0, e.v1)

    def __iter__(self):
        return (i for i in (self.v0, self.v1, self.v2))

    def __repr__(self):
        class_name = type(self).__name__
        return '{}({!r}, {!r}, {!r})'.format(class_name, *self)

class Circle:

    def __init__(self, center, radius):
        self.center = center
        self.radius = radius

    def __iter__(self):
        return (i for i in (self.center.x, self.center.y, self.radius))

    def __repr__(self):
        class_name = type(self).__name__
        return '{}({!r}, {!r})'.format(class_name, self.center, self.radius)

def circumcircle(triangle):
    v1, v2, v3 = triangle
    x1, y1 = v1
    x2, y2 = v2
    x3, y3 = v3
    absy1y2 = abs(y1 - y2)
    absy2y3 = abs(y2 - y3)

    if absy1y2 < EPSION and absy2y3 < EPSION:
        return

    if absy1y2 < EPSION:
        m2 = -(x3-x2)/(y3-y2)
        mx2 = (x2+x3)/2
        my2 = (y2+y3)/2
        xc = (x2+x1)/2
        yc = m2 *(xc-mx2) + my2
    elif absy2y3 < EPSION:
        m1 = -(x2-x1)/(y2-y1)
        mx1 = (x1+x2)/2
        my1 = (y1+y2)/2
        xc = (x2+x3)/2
        yc = m1*(xc - mx1) + my1
    else:
        m1 = -(x2-x1)/(y2-y1)
        m2 = -(x3-x2)/(y3-y2)
        mx1 = (x1+x2)/2
        mx2 = (x2+x3)/2
        my1 = (y1+y2)/2
        my2 = (y2+y3)/2
        xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2)
        yc =  m1*(xc - mx1) + my1 if (absy1y2 > absy2y3) else m2 * (xc - mx2) + my2

    dx = x2 - xc
    dy = y2 - yc
    return Circle(Point(xc, yc), math.sqrt(dx*dx+dy*dy))


width, height = 800, 300
im = Image.new("RGB", (width, height), (255,255,255))
draw = ImageDraw.Draw(im)

# generate triangles
num_triangles = 4
triangles = []

for i in range(num_triangles):
    left = i * width/num_triangles
    right = (i+1)*width/num_triangles
    v0 = Point(random.randint(left, right), random.randint(50, 250))
    v1 = Point(random.randint(left, right), random.randint(50, 250))
    v2 = Point(random.randint(left, right), random.randint(50, 250))
    triangle = Triangle(v0, v1, v2)
    print(triangle)
    print(circumcircle(triangle))
    triangles.append(triangle)


for triangle in triangles:
    v0, v1, v2 = triangle
    print('triangle2', triangle)
    circle = circumcircle(triangle)
    if circle:
        x,y,r = circle
        print('x, y, r', x, y, r)
        cr = random.randint(0,255)
        cg = random.randint(0,255)
        cb = random.randint(0,255)
        draw.polygon([(v0.x, v0.y), (v1.x, v1.y), (v2.x, v2.y)], fill = (cr,cg,cb))
        draw.ellipse((x-r, y-r, x+r, y+r), outline=(cr,cg,cb))


im.show()
