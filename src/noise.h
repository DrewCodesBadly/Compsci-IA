#ifndef NOISE_H
#define NOISE_H
#include "vec2.h"
class Noise
{
private:
    double frequency; // size of square areas in noise
    int seed;         // used in hash
    Vec2 randvec(Vec2 v);

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