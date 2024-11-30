#ifndef NOISE_H
#define NOISE_H
#include "vec2.h"
class Noise
{
private:
    double frequency;                              // size of square areas in noise
    int seed;                                      // used in hash
    const Vec2 rand_const{Vec2{3.54536, 6.44565}}; // random number used in random vector generation
    Vec2 hash(Vec2 v);
    double mixCorners(Vec2 a, Vec2 b, Vec2 c, Vec2 d);
    Vec2 randvec(double s);

public:
    ~Noise();
    Noise(int seed = 0, double scale = 1.0);
    double getValue(double x, double y);
    double getValue(Vec2 v);
};

#endif