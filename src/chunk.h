#ifndef CHUNK_H
#define CHUNK_H
#include <vector>
#include "terrain_object.h"
#include "terrain_generator.h"

class Chunk
{
private:
    bool tunnel;
    std::vector<TerrainObject> objects;
    Biome biome;

public:
};

#endif