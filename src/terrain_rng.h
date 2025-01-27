#include <random>

#ifndef RNG_H
#define RNG_H

class TerrainRNG
{
private:
    unsigned int current;
    std::mt19937 mt;

public:
    TerrainRNG(unsigned int seed = 0);
    ~TerrainRNG();
    unsigned int next();
    void set(int seed);
};

#endif