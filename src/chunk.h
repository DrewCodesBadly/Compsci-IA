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
    Chunk();
    ~Chunk();
    void add_object(TerrainObject o);
    void generate(godot::TileMapLayer *map, int x, int y, godot::TerrainGenerator *generator);
};

#endif