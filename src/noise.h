#ifndef NOISE_H
#define NOISE_H
#include "vec2.h"
class Noise
{
private:
    double scale; // size of square areas in noise
    int seed;     // used in hash
    Vec2 hash(Vec2 v);
    double mixCorners(Vec2 a, Vec2 b, Vec2 c, Vec2 d);

public:
    ~Noise();
    Noise(int seed = 0, double scale = 1.0);
    double getValue(int x, int y);
    double getValue(Vec2 v);
};

#endif