#include "noise.h"
#include "vec2.h"

Noise::Noise(int seed, double scale)
    : seed{seed}, scale{scale}
{
}

Noise::~Noise() {}

double Noise::getValue(Vec2 v)
{
    v.vecFloor();

    return 0.0;
}