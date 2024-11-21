#ifndef VEC2_H
#define VEC2_H

class Vec2
{
public:
    double x;
    double y;
    double dot(Vec2 v);
    void vecFloor();
    Vec2(double n);
    Vec2(double x, double y);
    ~Vec2();
};

#endif