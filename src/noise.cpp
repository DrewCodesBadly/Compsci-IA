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

// For convenience
double Noise::getValue(double x, double y)
{
    return getValue(Vec2{x, y});
}

// MODIFIES INPUT VECTOR!!!! Can be changed if this is an issue
/* See https://thebookofshaders.com/11/, https://gpfault.net/posts/perlin-noise.txt.html,
   and https://en.wikipedia.org/wiki/Perlin_noise for more info on noise */
double Noise::getValue(Vec2 v)
{
    // Multiply input vector by frequency
    v.x *= frequency;
    v.y *= frequency;

    // Get the 4 corners surrounding this point
    Vec2 bl{v.vecFloor()}; // take floor of v
    Vec2 tl{bl.x, bl.y + 1.0}, tr{bl.x + 1.0, bl.y + 1.0}, br{bl.x + 1.0, bl.y};

    // Also get the fractional point between the corners
    Vec2 fract{v.x - bl.x, v.y - bl.y};

    // Generate pseudo-random unit vectors corresponding to each corner
    bl = randvec(bl.dot(rand_const) + seed);
    tl = randvec(tl.dot(rand_const) + seed);
    tr = randvec(tr.dot(rand_const) + seed);
    br = randvec(br.dot(rand_const) + seed);

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

Vec2 randvec(double s)
{
    // Random number generator based on input seed in range [0, 2PI]
    // Hopefully its random enough i just kinda threw it together
    double angle = fmod(sin(s / 15.234) * 234.63465, 2 * PI);
    // Create a vector from the random angle
    return Vec2{cos(angle), sin(angle)};
}