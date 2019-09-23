#
# Be careful, might be buggy. Didn't do enough test for the code
# and didn't check for degenerate cases.
#

import random
import collections
import math
from PIL import Image, ImageDraw

EPSION = 1e-15

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

class Edge:

    def __init__(self, v0, v1):
        v0, v1 = sorted([v0, v1])
        self.v0 = v0
        self.v1 = v1

    def __eq__(self, other):
        return tuple(self) == tuple(other)

    def __hash__(self):
        return hash(tuple(self))

    def __iter__(self):
        return (i for i in (self.v0, self.v1))

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
         return Circle(Point(0,0),10000)
    #     #raise ValueError('Please check points')

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

class Delaunay:
    def __init__(self, vertices):
        self.vertices = vertices

    @staticmethod
    def supertriangle(vertices):
        xmin = vertices[0].x
        xmax = vertices[0].x
        ymin = vertices[0].y
        ymax = vertices[0].y

        for point in vertices:
            if point.y > ymax:
                ymax = point.y
            if point.y < ymin:
                ymin = point.y
            if point.x > xmax:
                xmax = point.x
            if point.x < xmin:
                xmin = point.x

        xl = xmax - xmin
        yl = ymax - ymin
        xlh = xl / 2

        return Triangle(Point(xmin - xlh - 2, ymax+1),
                        Point(xmin + xlh, ymin - yl),
                        Point(xmax + xlh + 2, ymax + 1))

    @staticmethod
    def point_circle_relation(point, circle):
        xc, yc = circle.center
        x, y = point
        r = circle.radius
        if (x - xc)**2 + (y-yc)**2 <= r**2:
            return 'Inside'
        elif x > xc + r:
            return 'Right'
        else:
            return 'Outside'

    def delaunay(self):
        vertices = sorted(self.vertices)
        print(vertices)
        super = Delaunay.supertriangle(vertices)
        print(super)
        s0, s1, s2 = super
        print(s0, s1, s2)
        temp_triangles = [super]
        print(circumcircle(super))
        triangles = [super]
        vertices.extend([s0, s1, s2])
        for vertex in vertices:
            edges = set()
            # do reverse because will delete while iterating
            for triangle in reversed(temp_triangles):
                circle = circumcircle(triangle)
                relation = Delaunay.point_circle_relation(vertex, circle)
                if relation == 'Right':
                    triangles.append(triangle)
                    temp_triangles.remove(triangle)
                    continue
                elif relation == 'Outside':
                    continue
                else:
                    v0, v1, v2 = triangle
                    e1, e2, e3 = Edge(v0,v1), Edge(v0,v2), Edge(v1, v2)
                    edges.add(e1) if e1 not in edges else edges.remove(e1)
                    edges.add(e2) if e2 not in edges else edges.remove(e2)
                    edges.add(e3) if e3 not in edges else edges.remove(e3)
                    temp_triangles.remove(triangle)

            for edge in edges:
                temp_triangles.append(Triangle.from_vertex_and_edge(vertex, edge))


        triangles = triangles + [tri for tri in temp_triangles if tri not in triangles]

        triangles = [tri for tri in triangles if s0 not in tri]
        triangles = [tri for tri in triangles if s1 not in tri]
        triangles = [tri for tri in triangles if s2 not in tri]

        return triangles



# generate triangles

width, height = 600, 600
im = Image.new("RGB", (width, height), (255,255,255))
draw = ImageDraw.Draw(im)

# generate triangles
points = []

num_x_cell = 5
num_y_cell = 5

cell_width = width/num_x_cell
cell_height = height/num_y_cell

for i in range(num_x_cell):
    for j in range(num_y_cell):
        left = cell_width*i
        right = left + cell_width
        up = cell_height*j
        down = up + cell_height
        p = Point(random.randint(left, right), random.randint(up, down))
        points.append(p)
        if random.random() > 0.5: #generate 1 more points if
            p = Point(random.randint(left, right), random.randint(up, down))
            points.append(p)

print(points)
triangles = Delaunay(points).delaunay()
print(triangles)

f2d = open("points2d.csv", "w")
f3d = open("points3d.csv", "w")

for triangle in triangles:
    v0, v1, v2 = triangle
    # x, y, r = circumcircle(triangle)
    x0, y0 = v0.x - width/2, v0.y - height/2
    x1, y1 = v1.x - width/2, v1.y - height/2
    x2, y2 = v2.x - width/2, v2.y - height/2

    f2d.write(str(x0) +',' + str(y0) + ', 0 \n')
    f2d.write(str(x1) +',' + str(y1) + ', 0 \n')
    f2d.write(str(x2) +',' + str(y2) + ', 0 \n')

    f3d.write(str(x0) +',' + str(y0) + ',' + str(x0**2+y0**2) + '\n')
    f3d.write(str(x1) +',' + str(y1) + ',' + str(x1**2+y1**2) + '\n')
    f3d.write(str(x2) +',' + str(y2) + ',' + str(x2**2+y2**2) + '\n')


    cr = random.randint(0,255)
    cg = random.randint(0,255)
    cb = random.randint(0,255)
    draw.polygon([(v0.x, v0.y), (v1.x, v1.y), (v2.x, v2.y)], outline = 'black')
    # draw.ellipse((x-r, y-r, x+r, y+r), outline=(cr,cg,cb))



im.show()
