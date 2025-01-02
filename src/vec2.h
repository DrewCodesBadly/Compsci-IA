#ifndef VEC2_H
#define VEC2_H

class Vec2
{
public:
    // Note x and y are public for easy vector math
    double x;
    double y;
    double dot(Vec2 v);
    double dist(Vec2 v);
    Vec2 vecFloor();
    Vec2(double n);
    Vec2(double x, double y);
    ~Vec2();
};

#endif