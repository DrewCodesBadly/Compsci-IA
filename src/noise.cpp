#include "noise.h"
#include "vec2.h"
#include <cmath>
#include <time.h>

#define PI 3.14159265 // close enough

Noise::Noise(int seed, double frequency)
    : seed{seed}, frequency{frequency}
{
}

Noise::~Noise() {}

double Noise::get_frequency() const
{
    return frequency;
}

void Noise::set_frequency(const double f)
{
    frequency = f;
}

void Noise::set_seed(const int s)
{
    seed = s;
}

int Noise::get_seed() const
{
    return seed;
}

// For convenience
double Noise::get_value(double x, double y)
{
    return get_value(Vec2{x, y});
}

// MODIFIES INPUT VECTOR!!!! Can be changed if this is an issue
/* See https://thebookofshaders.com/11/, https://gpfault.net/posts/perlin-noise.txt.html,
   and https://en.wikipedia.org/wiki/Perlin_noise for more info on noise */
double Noise::get_value(Vec2 v)
{
    // Multiply input vector by frequency
    v.x *= frequency;
    v.y *= frequency;

    // Get fractional and integer parts separately
    Vec2 bl{0.0, 0.0};
    Vec2 fract{modf(v.x, &bl.x), modf(v.y, &bl.y)};

    // Get the 4 corners surrounding this point
    Vec2 tl{bl.x, bl.y + 1.0}, tr{bl.x + 1.0, bl.y + 1.0}, br{bl.x + 1.0, bl.y};

    // Generate pseudo-random unit vectors corresponding to each corner
    bl = randvec(bl);
    tl = randvec(tl);
    tr = randvec(tr);
    br = randvec(br);

    // Calculate offset vectors and dot products for each corner
    double bl_dot, tl_dot, tr_dot, br_dot;
    bl_dot = bl.dot(fract);
    tl_dot = tl.dot(Vec2{fract.x, fract.y - 1.0});
    tr_dot = tr.dot(Vec2{fract.x - 1.0, fract.y - 1.0});
    br_dot = br.dot(Vec2{fract.x - 1.0, fract.y});

    // Smoothstep interpolate our fractional part
    fract.x = fract.x * fract.x * (3.0 - (2.0 * fract.x));
    fract.y = fract.y * fract.y * (3.0 - (2.0 * fract.y));

    // Interpolate dot products to produce the result
    double bl_tl = ((1.0 - fract.y) * bl_dot) + (fract.y * tl_dot);
    double br_tr = ((1.0 - fract.y) * br_dot) + (fract.y * tr_dot);
    return ((1.0 - fract.x) * bl_tl) + (fract.x * br_tr);
}

Vec2 Noise::randvec(Vec2 v)
{
    // modified from this hash https://godotshaders.com/snippet/random-value/
    v.x += seed;
    v.y += seed;
    double temp;
    double angle{modf(sin(v.dot(Vec2{12.9898, 78.233})) * 43758.5453123, &temp) * 2.0 * PI};
    return Vec2{cos(angle), sin(angle)};
}