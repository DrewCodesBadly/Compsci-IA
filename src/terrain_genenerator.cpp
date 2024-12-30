#include "terrain_generator.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void TerrainGenerator::_bind_methods()
{
    // seed property
    ClassDB::bind_method(D_METHOD("get_seed"), &TerrainGenerator::get_seed);
    ClassDB::bind_method(D_METHOD("set_seed", "given_seed"), &TerrainGenerator::set_seed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "seed"), "set_seed", "get_seed");

    // Reference to tilemap this will generate to
    ClassDB::bind_method(D_METHOD("get_tile_map"), &TerrainGenerator::get_tile_map);
    ClassDB::bind_method(D_METHOD("set_tile_map", "tile_map"), &TerrainGenerator::set_tile_map);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "tile_map"), "set_tile_map", "get_tile_map");

    // size property
    ClassDB::bind_method(D_METHOD("get_size"), &TerrainGenerator::get_size);
    ClassDB::bind_method(D_METHOD("set_size", "s"), &TerrainGenerator::set_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");

    // chunk size property
    ClassDB::bind_method(D_METHOD("get_chunk_size"), &TerrainGenerator::get_chunk_size);
    ClassDB::bind_method(D_METHOD("set_chunk_size", "s"), &TerrainGenerator::get_chunk_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "chunk_size"), "set_chunk_size", "get_chunk_size");

    // Biome frequency property
    ClassDB::bind_method(D_METHOD("get_biome_frequency"), &TerrainGenerator::get_biome_frequency);
    ClassDB::bind_method(D_METHOD("set_biome_frequency", "f"), &TerrainGenerator::set_biome_frequency);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "biome_frequency"), "set_biome_frequency", "get_biome_frequency");

    // link gen method
    ClassDB::bind_method(D_METHOD("generate"), &TerrainGenerator::generate);

    // export properties for tiles
    // source id
    ClassDB::bind_method(D_METHOD("get_tile_source_id"), &TerrainGenerator::get_tile_source_id);
    ClassDB::bind_method(D_METHOD("set_tile_source_id", "id"), &TerrainGenerator::set_tile_source_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "tile_source_id"), "set_tile_source_id", "get_tile_source_id");
    // floor, wall, etc
    ClassDB::bind_method(D_METHOD("get_floor_tile"), &TerrainGenerator::get_tile_source_id);
    ClassDB::bind_method(D_METHOD("set_floor_tile", "t"), &TerrainGenerator::set_tile_source_id);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "floor_tile"), "set_floor_tile", "get_floor_tile");
    ClassDB::bind_method(D_METHOD("get_wall_tile_organic"), &TerrainGenerator::get_tile_source_id);
    ClassDB::bind_method(D_METHOD("set_wall_tile_organic", "t"), &TerrainGenerator::set_tile_source_id);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "wall_tile_organic"), "set_wall_tile_organic", "get_wall_tile_organic");
    ClassDB::bind_method(D_METHOD("get_wall_tile_hybrid"), &TerrainGenerator::get_tile_source_id);
    ClassDB::bind_method(D_METHOD("set_wall_tile_hybrid", "t"), &TerrainGenerator::set_tile_source_id);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "wall_tile_hybrid"), "set_wall_tile_hybrid", "get_wall_tile_hybrid");
    ClassDB::bind_method(D_METHOD("get_wall_tile_industrial"), &TerrainGenerator::get_tile_source_id);
    ClassDB::bind_method(D_METHOD("set_wall_tile_industrial", "t"), &TerrainGenerator::set_tile_source_id);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "wall_tile_industrial"), "set_wall_tile_industrial", "get_wall_tile_industrial");
    ClassDB::bind_method(D_METHOD("get_wall_tile_alien"), &TerrainGenerator::get_tile_source_id);
    ClassDB::bind_method(D_METHOD("set_wall_tile_alien", "t"), &TerrainGenerator::set_tile_source_id);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "wall_tile_alien"), "set_wall_tile_alien", "get_wall_tile_alien");

    ClassDB::bind_method(D_METHOD("test_noise", "v"), &TerrainGenerator::test_noise);
}

TerrainGenerator::TerrainGenerator(int given_seed, double biome_frequency, int source_id)
    : seed{given_seed}, biome_noise{Noise{seed, biome_frequency}}, tile_source_id{source_id}
{
}

TerrainGenerator::~TerrainGenerator()
{
    // any needed cleanup here
}

void TerrainGenerator::set_seed(const int given_seed)
{
    seed = given_seed;
    biome_noise.set_seed(given_seed);
}

int TerrainGenerator::get_seed() const
{
    return seed;
}

void TerrainGenerator::set_tile_source_id(const int id)
{
    tile_source_id = id;
}

int TerrainGenerator::get_tile_source_id() const
{
    return tile_source_id;
}

void TerrainGenerator::set_floor_tile(const Vector2i t)
{
    floor_tile = t;
}

Vector2i TerrainGenerator::get_floor_tile() const
{
    return floor_tile;
}

void TerrainGenerator::set_wall_tile_organic(const Vector2i t)
{
    wall_tile_organic = t;
}

Vector2i TerrainGenerator::get_wall_tile_organic() const
{
    return wall_tile_organic;
}

void TerrainGenerator::set_wall_tile_hybrid(const Vector2i t)
{
    wall_tile_hybrid = t;
}

Vector2i TerrainGenerator::get_wall_tile_hybrid() const
{
    return wall_tile_hybrid;
}

void TerrainGenerator::set_wall_tile_industrial(const Vector2i t)
{
    wall_tile_industrial = t;
}

Vector2i TerrainGenerator::get_wall_tile_industrial() const
{
    return wall_tile_industrial;
}

void TerrainGenerator::set_wall_tile_alien(const Vector2i t)
{
    wall_tile_alien = t;
}

Vector2i TerrainGenerator::get_wall_tile_alien() const
{
    return wall_tile_alien;
}

void TerrainGenerator::set_biome_frequency(const double f)
{
    biome_noise.set_frequency(f);
}

double TerrainGenerator::get_biome_frequency() const
{
    return biome_noise.get_frequency();
}

void TerrainGenerator::set_size(const Vector2i s)
{
    size = s;
}

Vector2i TerrainGenerator::get_size() const
{
    return size;
}

NodePath TerrainGenerator::get_tile_map() const
{
    return tile_map;
}

void TerrainGenerator::set_tile_map(const NodePath &new_map)
{
    tile_map = new_map;
}

void TerrainGenerator::set_chunk_size(const Vector2i s)
{
    size = s;
}

Vector2i TerrainGenerator::get_chunk_size() const
{
    return size;
}

// TODO: ADD
void TerrainGenerator::generate()
{
    // Get a proprer reference to the tile map from the stored node path
    Node *map_node = get_node_or_null(tile_map);
    ERR_FAIL_NULL_MSG(map_node, "invalid tile map node path given");
    TileMapLayer *map = Object::cast_to<TileMapLayer>(map_node);
    ERR_FAIL_NULL_MSG(map_node, "tile map path does not point to a TileMapLayer");

    // Clear tilemap before generating new stuff
    map->clear();

    // Noise output tester
    for (int x{0}; x < size.x; ++x)
    {
        for (int y{0}; y < size.y; ++y)
        {
            // personal note: . derefs from a class, -> derefs from a pointer, data types with * are a pointer
            map->set_cell(
                Vector2i{x, y},
                tile_source_id,
                Vector2i{(int)((biome_noise.get_value(Vec2{(double)x, (double)y}) + 1.0) * 2.5), 0});
        }
    }

    // Scatter objects
}

// Add variable object sizes in the future?
/* Uses the poisson disk sampling algorithm, more information can be found at:
    - https://sighack.com/post/poisson-disk-sampling-bridsons-algorithm
*/
void TerrainGenerator::object_scatter(double r)
{
    const double cell_size{r / sqrt(2)};
    const int cells_x{(int)ceil(size.x / cell_size) + 1};
    const int cells_y{(int)ceil(size.y / cell_size) + 1};
}

double TerrainGenerator::test_noise(Vector2i v)
{
    return biome_noise.get_value(v.x, v.y);
}