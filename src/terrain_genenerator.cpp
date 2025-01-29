#include "terrain_generator.h"
#include "terrain_rng.h"
#include "chunk.h"
#include <godot_cpp/core/class_db.hpp>
#include <vector>
#include <cmath>

#include "chunk.h" // makes the vscode error shut up, scons handles the build fine tho

#define PI 3.14159265 // close enough

using namespace godot;
using std::vector;

void TerrainGenerator::_bind_methods()
{
    // seed property
    ClassDB::bind_method(D_METHOD("get_seed"), &TerrainGenerator::get_seed);
    ClassDB::bind_method(D_METHOD("set_seed", "given_seed"), &TerrainGenerator::set_seed);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");

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
    ClassDB::bind_method(D_METHOD("set_chunk_size", "s"), &TerrainGenerator::set_chunk_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "chunk_size"), "set_chunk_size", "get_chunk_size");

    // Noise frequency properties
    ClassDB::bind_method(D_METHOD("get_biome_frequency"), &TerrainGenerator::get_biome_frequency);
    ClassDB::bind_method(D_METHOD("set_biome_frequency", "f"), &TerrainGenerator::set_biome_frequency);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "biome_frequency"), "set_biome_frequency", "get_biome_frequency");
    ClassDB::bind_method(D_METHOD("get_density_frequency"), &TerrainGenerator::get_density_frequency);
    ClassDB::bind_method(D_METHOD("set_density_frequency", "f"), &TerrainGenerator::set_density_frequency);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "density_frequency"), "set_density_frequency", "get_density_frequency");

    // link gen method
    ClassDB::bind_method(D_METHOD("generate"), &TerrainGenerator::generate);

    // object scatter related properties
    ClassDB::bind_method(D_METHOD("get_scatter_tries"), &TerrainGenerator::get_scatter_tries);
    ClassDB::bind_method(D_METHOD("set_scatter_tries", "t"), &TerrainGenerator::set_scatter_tries);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "scatter_tries"), "set_scatter_tries", "get_scatter_tries");
    ClassDB::bind_method(D_METHOD("get_small_object_radius"), &TerrainGenerator::get_small_object_radius);
    ClassDB::bind_method(D_METHOD("set_small_object_radius", "r"), &TerrainGenerator::set_small_object_radius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "small_object_radius"), "set_small_object_radius", "get_small_object_radius");
    ClassDB::bind_method(D_METHOD("get_large_object_radius"), &TerrainGenerator::get_large_object_radius);
    ClassDB::bind_method(D_METHOD("set_large_object_radius", "r"), &TerrainGenerator::set_large_object_radius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "large_object_radius"), "set_large_object_radius", "get_large_object_radius");

    // export properties for tiles
    // source id
    ClassDB::bind_method(D_METHOD("get_tile_source_id"), &TerrainGenerator::get_tile_source_id);
    ClassDB::bind_method(D_METHOD("set_tile_source_id", "id"), &TerrainGenerator::set_tile_source_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "tile_source_id"), "set_tile_source_id", "get_tile_source_id");
    // floor, wall, etc
    ClassDB::bind_method(D_METHOD("get_wall_tile_organic"), &TerrainGenerator::get_wall_tile_organic);
    ClassDB::bind_method(D_METHOD("set_wall_tile_organic", "t"), &TerrainGenerator::set_wall_tile_organic);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "wall_tile_organic"), "set_wall_tile_organic", "get_wall_tile_organic");
    ClassDB::bind_method(D_METHOD("get_wall_tile_hybrid"), &TerrainGenerator::get_wall_tile_hybrid);
    ClassDB::bind_method(D_METHOD("set_wall_tile_hybrid", "t"), &TerrainGenerator::set_wall_tile_hybrid);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "wall_tile_hybrid"), "set_wall_tile_hybrid", "get_wall_tile_hybrid");
    ClassDB::bind_method(D_METHOD("get_wall_tile_industrial"), &TerrainGenerator::get_wall_tile_industrial);
    ClassDB::bind_method(D_METHOD("set_wall_tile_industrial", "t"), &TerrainGenerator::set_wall_tile_industrial);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "wall_tile_industrial"), "set_wall_tile_industrial", "get_wall_tile_industrial");
    ClassDB::bind_method(D_METHOD("get_wall_tile_alien"), &TerrainGenerator::get_wall_tile_alien);
    ClassDB::bind_method(D_METHOD("set_wall_tile_alien", "t"), &TerrainGenerator::set_wall_tile_alien);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "wall_tile_alien"), "set_wall_tile_alien", "get_wall_tile_alien");

    ClassDB::bind_method(D_METHOD("get_floor_tile_organic"), &TerrainGenerator::get_floor_tile_organic);
    ClassDB::bind_method(D_METHOD("set_floor_tile_organic", "t"), &TerrainGenerator::set_floor_tile_organic);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "floor_tile_organic"), "set_floor_tile_organic", "get_floor_tile_organic");
    ClassDB::bind_method(D_METHOD("get_floor_tile_hybrid"), &TerrainGenerator::get_floor_tile_hybrid);
    ClassDB::bind_method(D_METHOD("set_floor_tile_hybrid", "t"), &TerrainGenerator::set_floor_tile_hybrid);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "floor_tile_hybrid"), "set_floor_tile_hybrid", "get_floor_tile_hybrid");
    ClassDB::bind_method(D_METHOD("get_floor_tile_industrial"), &TerrainGenerator::get_floor_tile_industrial);
    ClassDB::bind_method(D_METHOD("set_floor_tile_industrial", "t"), &TerrainGenerator::set_floor_tile_industrial);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "floor_tile_industrial"), "set_floor_tile_industrial", "get_floor_tile_industrial");
    ClassDB::bind_method(D_METHOD("get_floor_tile_alien"), &TerrainGenerator::get_floor_tile_alien);
    ClassDB::bind_method(D_METHOD("set_floor_tile_alien", "t"), &TerrainGenerator::set_floor_tile_alien);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "floor_tile_alien"), "set_floor_tile_alien", "get_floor_tile_alien");

    // enabled biomes
    ClassDB::bind_method(D_METHOD("get_alien_enabled"), &TerrainGenerator::get_alien_enabled);
    ClassDB::bind_method(D_METHOD("set_alien_enabled", "b"), &TerrainGenerator::set_alien_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "alien_enabled"), "set_alien_enabled", "get_alien_enabled");
    ClassDB::bind_method(D_METHOD("get_industrial_enabled"), &TerrainGenerator::get_industrial_enabled);
    ClassDB::bind_method(D_METHOD("set_industrial_enabled", "b"), &TerrainGenerator::set_industrial_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "industrial_enabled"), "set_industrial_enabled", "get_industrial_enabled");
    ClassDB::bind_method(D_METHOD("get_organic_enabled"), &TerrainGenerator::get_organic_enabled);
    ClassDB::bind_method(D_METHOD("set_organic_enabled", "b"), &TerrainGenerator::set_organic_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "organic_enabled"), "set_organic_enabled", "get_organic_enabled");
    ClassDB::bind_method(D_METHOD("get_hybrid_enabled"), &TerrainGenerator::get_hybrid_enabled);
    ClassDB::bind_method(D_METHOD("set_hybrid_enabled", "b"), &TerrainGenerator::set_hybrid_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hybrid_enabled"), "set_hybrid_enabled", "get_hybrid_enabled");

    ClassDB::bind_method(D_METHOD("test_noise", "v"), &TerrainGenerator::test_noise);
}

TerrainGenerator::TerrainGenerator()
{
    // Initialize objects needed
    biome_noise = Noise(seed, 1.0);
    density_noise = Noise(seed + 1, 1.0);
}

TerrainGenerator::~TerrainGenerator()
{
    // any needed cleanup here
}

// TODO: ADD
void TerrainGenerator::generate()
{

    // Create main rng object
    TerrainRNG main_rng{(unsigned int)seed};

    // Create and initialize 2d array of empty chunks, setting their biomes appropriately
    chunks.clear();
    Vector2i map_size_chunks{Vector2i(ceil((double)size.x / chunk_size.x), ceil((double)size.y / chunk_size.y))};
    // Constants for determining biomes (awful code please ignore)
    int biomes_enabled{0};
    vector<Biome> biomes;
    if (organic_enabled)
    {
        biomes_enabled++;
        biomes.push_back(Biome::ORGANIC);
    }
    if (alien_enabled)
    {
        biomes_enabled++;
        biomes.push_back(Biome::ALIEN);
    }
    if (hybrid_enabled)
    {
        biomes_enabled++;
        biomes.push_back(Biome::HYBRID);
    }
    if (industrial_enabled)
    {
        biomes_enabled++;
        biomes.push_back(Biome::INDUSTRIAL);
    }
    // Quit out if no biomes enabled
    ERR_FAIL_COND_MSG(biomes_enabled < 1, "no biomes! must enable at least 1 to generate a map");

    for (int x{0}; x < map_size_chunks.x; x++)
    {
        vector<Chunk> v;
        for (int y{0}; y < map_size_chunks.y; y++)
        {
            // normalize to [0, 1)
            double biome_noise_val{(biome_noise.get_value((double)x, (double)y) / 2.0) + 1.0};
            if (biome_noise_val == 1.0)
                biome_noise_val = 0.9999;
            // then to integers in range [0, biomes_enabled) so we can reference an array of enabled biome types
            biome_noise_val *= biomes_enabled;

            v.push_back(Chunk(biomes.at((unsigned int)biome_noise_val)));
        }
        chunks.push_back(v);
    }

    // Get a proprer reference to the tile map from the stored node path
    Node *map_node = get_node_or_null(tile_map);
    ERR_FAIL_NULL_MSG(map_node, "invalid tile map node path given");
    TileMapLayer *map = Object::cast_to<TileMapLayer>(map_node);
    ERR_FAIL_NULL_MSG(map_node, "tile map path does not point to a TileMapLayer");

    // Clear tilemap before generating new stuff
    map->clear();

    // Noise output tester
    // for (int x{0}; x < size.x; ++x)
    // {
    //     for (int y{0}; y < size.y; ++y)
    //     {
    //         // personal note: . derefs from a class, -> derefs from a pointer, data types with * are a pointer
    //         map->set_cell(
    //             Vector2i{x, y},
    //             tile_source_id,
    //             Vector2i{(int)((biome_noise.get_value(Vec2{(double)x, (double)y}) + 1.0) * 2.5), 0});
    //     }
    // }

    // Object scatter passes
    // Pass 1: scatter small objects
    object_scatter(small_object_radius, main_rng, scatter_tries);

    // // Pass 2: scatter large objects
    // // Should naturally draw over small objects, so behavior will look about normal
    object_scatter(large_object_radius, main_rng, scatter_tries);

    // Remove objects to change object density chunk to chunk
    // TODO: Add

    // Designate tunnel chunks
    // TODO: add

    // Generate chunks
    for (int x{0}; x < chunks.size(); x++)
    {
        for (int y{0}; y < chunks[0].size(); y++)
        {
            chunks[x][y].generate(map, x, y, this);
        }
    }
}

// used in object_scatter
void TerrainGenerator::insert_object(vector<vector<Vec2>> &grid, Vec2 p, double cellsize, double obj_size)
{
    const int pos_x{(int)(p.x / cellsize)};
    const int pos_y{(int)(p.y / cellsize)};
    grid[pos_x][pos_y] = p;

    // Add object to appropriate chunk
    // Integer division takes the floor by default
    // Note that when object positions go from doubles to truncated ints some precision is lost, but it should be good enough
    chunks[p.x / chunk_size.x][p.y / chunk_size.y].add_object(TerrainObject(Vector2i(p.x, p.y), obj_size));
}

// Add variable object sizes in the future?
/* Uses the poisson disk sampling algorithm, more information can be found at:
    - https://sighack.com/post/poisson-disk-sampling-bridsons-algorithm
*/
// k is number of attempts before the algorithm gives up
// Note that the radius used will be double the object size
void TerrainGenerator::object_scatter(double r, TerrainRNG main_rng, int k)
{
    const double cell_size{r / sqrt(2)};
    const int cells_x{(int)ceil(size.x / cell_size) + 1};
    const int cells_y{(int)ceil(size.y / cell_size) + 1};

    // might be unneeded, we'll see how output works best
    // initialize to -1, 0 vectors
    // vector<vector<Vec2>> grid;
    // for (int x{0}; x < cells_x; ++x)
    // {
    //     vector<Vec2> v;
    //     for (int y{0}; y < cells_y; ++y)
    //     {
    //         v.push_back(Vec2{-1.0, 0.0});
    //     }
    //     grid.push_back(v);
    // }
    Vec2 grid[cells_x][cells_y];

    // Initial point
    vector<Vec2> active;
    Vec2 p0{(double)(main_rng.next() % size.x), (double)(main_rng.next() % size.y)};
    insert_object(grid, p0, cell_size, r / 2.0);
    active.push_back(p0);

    bool success;
    while (active.size() > 0)
    {
        // Pick a random active point
        unsigned long long idx{main_rng.next() % active.size()};
        Vec2 point = active[idx];

        // Try up to k times to find a new point
        success = false;
        for (int i{0}; i < k; ++i)
        {
            // Pick point random angle away between r and 2r distance
            // Uses a random angle + distance and basic trig
            double angle{(double)main_rng.next() / UINT_MAX * 2 * PI};
            double dist{(double)main_rng.next() / UINT_MAX * r + r};
            Vec2 new_point{point.x + (dist * cos(angle)), point.y + (dist * sin(angle))};

            // Check neighboring cells to determine if this point is valid
            // Code will continue as soon as it determines the point is invalid, otherwise set success to true
            // Check if point is inbounds
            if (new_point.x < 0 || new_point.y < 0 || new_point.x >= size.x || new_point.y >= size.y)
                continue;

            // Check 8 neighbor cells
            bool valid{true};
            int x{(int)(new_point.x / cell_size)}, y{(int)(new_point.y / cell_size)};
            int x_min{std::max(x - 1, 0)}, x_max{std::min(x + 1, cells_x - 1)};
            int y_min{std::max(y - 1, 0)}, y_max{std::min(y + 1, cells_y - 1)};
            for (int x{x_min}; x <= x_max; ++x)
            {
                for (int y{y_min}; y <= y_max; ++y)
                {
                    if (grid[x][y].x != -1.0 && new_point.dist(grid[x][y]) < r)
                        valid = false;
                }
            }
            if (!valid)
                continue;

            // Nothing went wrong so we add the point and break out of the for loop
            active.push_back(new_point);
            insert_object(grid, new_point, cell_size, r / 2.0);
            success = true;
            break;
        }

        // Add new point or remove p from active
        if (!success)
        {
            active.erase(active.begin() + idx);
        }
    }
}

// Boilerplate hidden for your convenience (mutators/accessors)
double TerrainGenerator::test_noise(Vector2i v)
{
    return biome_noise.get_value(v.x, v.y);
}

void TerrainGenerator::set_scatter_tries(const int t)
{
    scatter_tries = t;
}
int TerrainGenerator::get_scatter_tries() const
{
    return scatter_tries;
}
void TerrainGenerator::set_small_object_radius(const double r)
{
    small_object_radius = r;
}
double TerrainGenerator::get_small_object_radius() const
{
    return small_object_radius;
}
void TerrainGenerator::set_large_object_radius(const double r)
{
    large_object_radius = r;
}
double TerrainGenerator::get_large_object_radius() const
{
    return large_object_radius;
}

void TerrainGenerator::set_density_frequency(const double f)
{
    density_noise.set_frequency(f);
}

double TerrainGenerator::get_density_frequency() const
{
    return density_noise.get_frequency();
}

void TerrainGenerator::set_seed(const int given_seed)
{
    seed = given_seed;
    // Also update other objects
    biome_noise.set_seed(given_seed);
    density_noise.set_seed(given_seed + 1);
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

void TerrainGenerator::set_floor_tile_organic(const Vector2i t)
{
    floor_tile_organic = t;
}

Vector2i TerrainGenerator::get_floor_tile_organic() const
{
    return floor_tile_organic;
}

void TerrainGenerator::set_floor_tile_hybrid(const Vector2i t)
{
    floor_tile_hybrid = t;
}

Vector2i TerrainGenerator::get_floor_tile_hybrid() const
{
    return floor_tile_hybrid;
}

void TerrainGenerator::set_floor_tile_industrial(const Vector2i t)
{
    floor_tile_industrial = t;
}

Vector2i TerrainGenerator::get_floor_tile_industrial() const
{
    return floor_tile_industrial;
}

void TerrainGenerator::set_floor_tile_alien(const Vector2i t)
{
    floor_tile_alien = t;
}

Vector2i TerrainGenerator::get_floor_tile_alien() const
{
    return floor_tile_alien;
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
    chunk_size = s;
}

Vector2i TerrainGenerator::get_chunk_size() const
{
    return chunk_size;
}

void TerrainGenerator::set_alien_enabled(const bool e)
{
    alien_enabled = e;
}

bool TerrainGenerator::get_alien_enabled() const
{
    return alien_enabled;
}

void TerrainGenerator::set_organic_enabled(const bool e)
{
    organic_enabled = e;
}

bool TerrainGenerator::get_organic_enabled() const
{
    return organic_enabled;
}

void TerrainGenerator::set_industrial_enabled(const bool e)
{
    industrial_enabled = e;
}

bool TerrainGenerator::get_industrial_enabled() const
{
    return industrial_enabled;
}

void TerrainGenerator::set_hybrid_enabled(const bool e)
{
    hybrid_enabled = e;
}

bool TerrainGenerator::get_hybrid_enabled() const
{
    return hybrid_enabled;
}
