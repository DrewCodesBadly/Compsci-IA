#include "chunk.h"

Chunk::Chunk()
{
}

Chunk::Chunk(enum Biome b)
    : biome{b}
{
}

Chunk::~Chunk()
{
}

void Chunk::add_object(TerrainObject o)
{
    objects.push_back(o);
}

using namespace godot;
void Chunk::generate(TileMapLayer *map, int x, int y, TerrainGenerator *generator)
{
    int source_id = generator->get_tile_source_id();

    // Get appropriate tiles for the biome
    Vector2i floor_tile;
    Vector2i wall_tile;
    switch (biome)
    {
    case Biome::ALIEN:
        floor_tile = generator->get_floor_tile_alien();
        wall_tile = generator->get_wall_tile_alien();
        break;
    case Biome::INDUSTRIAL:
        floor_tile = generator->get_floor_tile_industrial();
        wall_tile = generator->get_wall_tile_industrial();
        break;
    case Biome::ORGANIC:
        floor_tile = generator->get_floor_tile_organic();
        wall_tile = generator->get_wall_tile_organic();
        break;
    case Biome::HYBRID:
        floor_tile = generator->get_floor_tile_hybrid();
        wall_tile = generator->get_wall_tile_hybrid();
        break;
    }

    Vector2i chunk_size{generator->get_chunk_size()};
    Vector2i top_right_cell{Vector2i(x, y) * chunk_size};

    // Fill whole chunk with floor tiles
    for (int local_x{0}; local_x < chunk_size.x; local_x++)
    {
        for (int local_y{0}; local_y < chunk_size.y; local_y++)
        {
            map->set_cell(top_right_cell + Vector2i(local_x, local_y), source_id, floor_tile);
        }
    }

    // Generate each object in this chunk's object list
    // TODO: Proper object generation
    for (TerrainObject o : objects)
    {
        map->set_cell(o.get_pos(), source_id, wall_tile);
    }
}