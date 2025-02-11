#ifndef CHUNK_H
#define CHUNK_H
#include <vector>
#include "terrain_object.h"
#include "biome.h"
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <terrain_generator.h>
#include "terrain_rng.h" // seems to already be included but just in case :3

using namespace godot;

namespace godot
{
    class TerrainChunk : public Resource
    {
        GDCLASS(TerrainChunk, Resource);

    private:
        Array wall_directions;
        bool empty = true;
        bool tunnel;
        std::vector<TerrainObject> objects;
        enum Biome biome;

    protected:
        static void _bind_methods();

    public:
        TerrainChunk(enum Biome b);
        ~TerrainChunk();
        void add_object(TerrainObject o);
        void set_non_empty();
        void generate(TileMapLayer *map, int x, int y, TerrainGenerator *generator);
        void remove_random_objects(int num, TerrainRNG main_rng);
        // Expected that enter and exit are cardinal directions e.g. (1, 0)
        void add_wall(Vector2i wall);
        bool is_tunnel() const;
        bool is_empty() const;
        enum Biome get_biome() const;
        int get_biome_godot_compat() const;
    };
}

#endif