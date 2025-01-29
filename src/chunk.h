#ifndef CHUNK_H
#define CHUNK_H
#include <vector>
#include "terrain_object.h"
#include "biome.h"
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <terrain_generator.h>

using namespace godot;

class Chunk
{
private:
    bool tunnel{false};
    std::vector<TerrainObject> objects;
    enum Biome biome;

public:
    Chunk();
    Chunk(enum Biome b);
    ~Chunk();
    void add_object(TerrainObject o);
    void generate(TileMapLayer *map, int x, int y, TerrainGenerator *generator);
};

#endif