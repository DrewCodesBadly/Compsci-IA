#ifndef CHUNK_H
#define CHUNK_H
#include <vector>
#include "terrain_object.h"
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <terrain_generator.h>

using namespace godot;

enum Biome
{
    INDUSTRIAL,
    ALIEN,
    HYBRID,
    ORGANIC
};

class Chunk
{
private:
    bool tunnel{false};
    std::vector<TerrainObject> objects;
    enum Biome biome;

public:
    Chunk();
    ~Chunk();
    void add_object(TerrainObject o);
    void generate(TileMapLayer *map, int x, int y, TerrainGenerator *generator);
};

#endif