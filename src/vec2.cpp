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

double Vec2::dist(Vec2 v)
{
    // Oh my god there's no way I actually wrote this, had me bamboozled for hours
    // return std::sqrt(std::pow(v.x, 2.0) + std::pow(v.y, 2.0));
    return std::sqrt(std::pow(v.x - x, 2.0) + std::pow(v.y - y, 2.0));
}

Vec2 Vec2::vecFloor()
{
    x = floor(x);
    y = floor(y);
    return *this;
}