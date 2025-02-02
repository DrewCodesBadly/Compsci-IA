#ifndef CHUNK_H
#define CHUNK_H
#include <vector>
#include "terrain_object.h"
#include "biome.h"
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <terrain_generator.h>
#include "terrain_rng.h" // seems to already be included but just in case :3

using namespace godot;

class Chunk
{
private:
    bool tunnel{false};
    std::vector<TerrainObject> objects;
    enum Biome biome;
    vector<Vector2i> exits;

public:
    Chunk();
    Chunk(enum Biome b);
    ~Chunk();
    void add_object(TerrainObject o);
    void generate(TileMapLayer *map, int x, int y, TerrainGenerator *generator);
    void remove_random_objects(int num, TerrainRNG main_rng);
    // Expected that enter and exit are cardinal directions e.g. (1, 0)
    void add_exit(Vector2i exit);
    void make_tunnel();
    bool is_tunnel() const;
    enum Biome get_biome() const;
};

#endif