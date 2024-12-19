#ifndef NOISE_H
#define NOISE_H
#include "vec2.h"
class Noise
{
private:
    double frequency; // size of square areas in noise
    int seed;         // used in hash
    Vec2 hash(Vec2 v);
    // double mix_corners(Vec2 a, Vec2 b, Vec2 c, Vec2 d);
    Vec2 randvec(double s);

public:
    ~Noise();
    Noise(int seed = 0, double scale = 1.0);
    double get_value(double x, double y);
    double get_value(Vec2 v);
    double get_frequency() const;
    void set_frequency(const double f);
    int get_seed() const;
    void set_seed(const int s);
};

#endif