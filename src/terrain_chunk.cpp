#include "terrain_chunk.h"

using namespace godot;

void TerrainChunk::_bind_methods()
{
}

TerrainChunk::TerrainChunk(enum Biome b)
    : biome{b}, tunnel{false}, exit_east{false}, exit_west{false}, exit_north{false}, exit_south{false}, empty{true}, wall_directions{Array()}
{
}

TerrainChunk::~TerrainChunk()
{
}

void TerrainChunk::add_object(TerrainObject o)
{
    objects.push_back(o);
}

void TerrainChunk::generate(TileMapLayer *map, int x, int y, TerrainGenerator *generator)
{
    if (empty)
        return;
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
    Vector2i top_left_cell{Vector2i(x, y) * chunk_size};

    // Fill whole chunk with floor tiles
    for (int local_x{0}; local_x < chunk_size.x; local_x++)
    {
        for (int local_y{0}; local_y < chunk_size.y; local_y++)
        {
            map->set_cell(top_left_cell + Vector2i(local_x, local_y), source_id, floor_tile);
        }
    }

    // Generate as a tunnel chunk
    if (tunnel)
    {
        // Try each side individually - draws lines at the edges (debug tunnel)
        if (!exit_north)
        {
            for (Vector2i v{top_left_cell}; v.x < top_left_cell.x + chunk_size.x; v.x++)
            {

                map->set_cell(v, source_id, wall_tile);
            }
        }
        if (!exit_south)
        {
            for (Vector2i v{top_left_cell + Vector2i(0, chunk_size.y - 1)}; v.x < top_left_cell.x + chunk_size.x; v.x++)
            {

                map->set_cell(v, source_id, wall_tile);
            }
        }
        if (!exit_west)
        {
            for (Vector2i v{top_left_cell}; v.y < top_left_cell.y + chunk_size.y; v.y++)
            {

                map->set_cell(v, source_id, wall_tile);
            }
        }
        if (!exit_east)
        {
            for (Vector2i v{top_left_cell + Vector2i(chunk_size.x - 1, 0)}; v.y < top_left_cell.y + chunk_size.y; v.y++)
            {

                map->set_cell(v, source_id, wall_tile);
            }
        }

        // For testing purposes, make the chunk all floor tiles - not used anymore (hopefully)
        // for (int local_x{0}; local_x < chunk_size.x; local_x += 2)
        // {
        //     for (int local_y{0}; local_y < chunk_size.y; local_y += 2)
        //     {
        //         map->set_cell(top_left_cell + Vector2i(local_x, local_y), source_id, wall_tile);
        //     }
        // }
    }

    // Standard generation
    else
    {
        // Generate each object in this chunk's object list
        // TODO: Proper object generation
        for (TerrainObject o : objects)
        {
            map->set_cell(o.get_pos(), source_id, wall_tile);
        }
    }
}

// Removes num objects at random (based on main_rng) from the objects vector
void TerrainChunk::remove_random_objects(int num, TerrainRNG main_rng)
{
    for (int i{0}; i < num && objects.size() > 0; i++)
    {
        objects.erase(objects.begin() + (main_rng.next() % objects.size()));
    }
}

void TerrainChunk::add_wall(Vector2i wall)
{
    // Match to the proper north, east, south, west boolean depending on the vector direction passed
    if (wall.x > 0)
    {
        wall_directions.append(Vector2i(1, 0));
    }
    else if (wall.x < 0)
    {
        wall_directions.append(Vector2i(-1, 0));
    }
    else if (wall.y > 0)
    {
        wall_directions.append(Vector2i(0, 1));
    }
    else
    {
        wall_directions.append(Vector2i(0, -1));
    }
}

enum Biome TerrainChunk::get_biome() const
{
    return biome;
}

bool TerrainChunk::is_tunnel() const
{
    return tunnel;
}
