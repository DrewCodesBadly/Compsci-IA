#include "vec2.h"
#include <cmath>

// member list initializations for vectors
Vec2::Vec2(double x, double y)
    : x{x}, y{y}
{
}

Vec2::Vec2(double n)
    : x{n}, y{n}
{
}

Vec2::~Vec2() {}

double Vec2::dot(Vec2 v)
{
    return (x * v.x) + (y * v.y);
}

Vec2 Vec2::vecFloor()
{
    x = floor(x);
    y = floor(y);
    return *this;
}