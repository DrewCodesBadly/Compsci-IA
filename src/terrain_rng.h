#include <random>

#ifndef RNG_H
#define RNG_H

class TerrainRNG
{
    private:
        int current;
        std::mt19937 mt;

    public:
        TerrainRNG(int seed = 0);
        ~TerrainRNG();
        int next();
        void set(int seed);
};

#endif