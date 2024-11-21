#include "vec2.h"
#include <cmath>

Vec2::Vec2(double x, double y)
    : x{x}, y{y}
{
}

Vec2::Vec2(double n)
    : x{x}, y{n}
{
}

Vec2::~Vec2() {}

double Vec2::dot(Vec2 v)
{
    return (x * v.x) + (y * v.y);
}

void Vec2::vecFloor()
{
    x = floor(x);
    y = floor(y);
}