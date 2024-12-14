#include "terrain_rng.h"
#include <random>

TerrainRNG::TerrainRNG(unsigned int seed)
    : current{seed}, mt{std::mt19937{seed}}
{
}

TerrainRNG::~TerrainRNG()
{
}

int TerrainRNG::next()
{
    current = mt();
    return current;
}

void TerrainRNG::set(int seed)
{
    current = seed;
    mt = std::mt19937{current};
}