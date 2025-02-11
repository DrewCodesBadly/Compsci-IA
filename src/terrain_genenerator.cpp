#include "terrain_generator.h"
#include "terrain_rng.h"
#include <godot_cpp/core/class_db.hpp>
#include <vector>
#include <cmath>

#include "terrain_chunk.h" // makes the vscode error shut up, scons handles the build fine tho

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
    ClassDB::bind_method(D_METHOD("get_max_removed_objects"), &TerrainGenerator::get_max_removed_objects);
    ClassDB::bind_method(D_METHOD("set_max_removed_objects", "m"), &TerrainGenerator::set_max_removed_objects);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_removed_objects"), "set_max_removed_objects", "get_max_removed_objects");

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

    // tunnels
    ClassDB::bind_method(D_METHOD("get_room_distance"), &TerrainGenerator::get_room_dist);
    ClassDB::bind_method(D_METHOD("set_room_distance", "d"), &TerrainGenerator::set_room_dist);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "room_distance"), "set_room_distance", "get_room_distance");
    ClassDB::bind_method(D_METHOD("get_room_size_max"), &TerrainGenerator::get_room_size_max);
    ClassDB::bind_method(D_METHOD("set_room_size_max", "l"), &TerrainGenerator::set_room_size_max);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "room_size_max"), "set_room_size_max", "get_room_size_max");
    ClassDB::bind_method(D_METHOD("get_room_size_min"), &TerrainGenerator::get_room_size_min);
    ClassDB::bind_method(D_METHOD("set_room_size_min", "l"), &TerrainGenerator::set_room_size_min);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "room_size_min"), "set_room_size_min", "get_room_size_min");

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

    ClassDB::bind_method(D_METHOD("world_to_nearest_chunk_coords", "world_coords"), &TerrainGenerator::world_to_nearest_chunk_coords);
    ClassDB::bind_method(D_METHOD("chunk_to_world_coords", "chunk_coords"), &TerrainGenerator::chunk_to_world_coords);
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

Vector2i TerrainGenerator::world_to_nearest_chunk_coords(Vector2 world)
{
    return Vector2i((int)(world.x / chunk_size.x), (int)(world.y / chunk_size.y));
}

Vector2 TerrainGenerator::chunk_to_world_coords(Vector2i chunk_coords)
{
    return Vector2(chunk_coords * chunk_size);
}

// Takes an input in range [-1, 1] and transforms it to [0, 1), optionally trimming away the edges of the range
double normalize_and_trim(double input, double trim_fac = 2.0)
{
    double val{input / trim_fac + 0.5};
    // Clamp within [0, 1)
    if (val >= 1.0)
        val = 0.9999;
    else if (val <= 0)
        val = 0;
    return val;
}

// Entrypoint for map generation
void TerrainGenerator::generate()
{
    // Checks for valid parameters
    // Get a proprer reference to the tile map from the stored node path - print error and return if there is no valid TileMapLayer node provided
    Node *map_node = get_node_or_null(tile_map);
    ERR_FAIL_NULL_MSG(map_node, "Invalid tile map node path given!");
    TileMapLayer *map = Object::cast_to<TileMapLayer>(map_node);
    ERR_FAIL_NULL_MSG(map_node, "Tile map path does not point to a TileMapLayer!");
    ERR_FAIL_COND_MSG(room_size_min > room_size_max, "room_size_min must be greater than room_size_max!");
    ERR_FAIL_COND_MSG(scatter_tries <= 0, "Scatter tries must be greater than 0 for the scattering algorithm to work properly.");
    ERR_FAIL_COND_MSG(size.x <= 0 || size.y <= 0, "Map size must be >= 0 in both axes for a valid map to be generated.");
    ERR_FAIL_COND_MSG(chunk_size.x <= 0 || chunk_size.y <= 0, "TerrainChunk size must be >= 0 in both axes for a valid map to be generated.");
    ERR_FAIL_COND_MSG(room_size_max.x > room_size_min.x || room_size_max.y > room_size_min.y, "Max room size should be greater than min room size.");
    // Constants for determining biomes - also checks to make sure at least one biome is enabled
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
    // Error and return if no biomes are enabled
    ERR_FAIL_COND_MSG(biomes_enabled < 1, "no biomes! must enable at least 1 to generate a map");

    // Instantiate new rng object as the main rng
    main_rng = TerrainRNG((unsigned int)seed);

    // Create and initialize 2d array of empty chunks, setting their biomes appropriately
    chunks.clear();
    Vector2i map_size_chunks{Vector2i(ceil((double)size.x / chunk_size.x), ceil((double)size.y / chunk_size.y))};

    for (int x{0}; x < map_size_chunks.x; x++)
    {
        vector<TerrainChunk> v;
        for (int y{0}; y < map_size_chunks.y; y++)
        {
            // Normalize to [0, 1)
            // Also, to ensure a more even distribution of biomes edges of the range are trimmed a bit (actually pretty heavily)
            double biome_noise_val{normalize_and_trim(biome_noise.get_value(x, y), 1.25)};
            // Then to integers in range [0, biomes_enabled) so we can reference biomes
            biome_noise_val *= biomes_enabled;
            v.push_back(TerrainChunk(biomes[(int)biome_noise_val])); // int used instead of unsigned int to avoid crashes in case user inputs negative
        }
        chunks.push_back(v);
    }

    // Clear tilemap before generating new stuff
    map->clear();

    // Object scatter passes
    // Pass 1: scatter small objects
    object_scatter(small_object_radius, scatter_tries);

    // Pass 2: scatter large objects
    // Should naturally draw over small objects, so behavior will look about normal
    object_scatter(large_object_radius, scatter_tries);

    // Remove objects to change object density chunk to chunk
    for (int x{0}; x < chunks.size(); x++)
    {
        for (int y{0}; y < chunks[0].size(); y++)
        {
            // Normalized and trimmed just like for biome noise
            double val{normalize_and_trim(density_noise.get_value(x, y), 1.25)};
            // Then to integers in range [0, max_removed_objects), and that many objects are removed from the chunk.
            chunks[x][y].remove_random_objects((int)(val * max_removed_objects), main_rng);
        }
    }

    // Generate room positions and shapes, settings chunk wall positions
    Vector2i max_min_room_diff{room_size_max - room_size_min};
    vector<Vec2> room_points = point_scatter(room_dist, scatter_tries);
    for (Vec2 p : room_points)
    {
        Vector2i room_size{Vector2i((main_rng.next() % max_min_room_diff.x) + room_size_min.x, (main_rng.next() % max_min_room_diff.y) + room_size_min.y)};
        Vector2i top_left{world_to_nearest_chunk_coords(Vector2(p.x, p.y)) - (room_size / 2)}; // integer division is fine here
        if (top_left.x < 0)
            top_left.x = 0;
        if (top_left.y < 0)
            top_left.y = 0;
        Vector2i bottom_right{top_left + room_size};
        if (bottom_right.x < chunks.size() - 1)
            bottom_right.x = chunks.size() - 1;
        if (bottom_right.y < chunks[0].size() - 1)
            bottom_right.y = chunks[0].size() - 1;
        Vector2i current_chunk{top_left};

        // Iterate through all chunks in the room, creating walls and flagging them as not empty (actually part of the map)
        for (current_chunk.x; current_chunk.x <= bottom_right.x; current_chunk.x++)
        {
            for (current_chunk.y; current_chunk.y <= bottom_right.y; current_chunk.y++)
            {
                TerrainChunk c = chunks[current_chunk.x][current_chunk.y];
                c.set_non_empty();
                if (current_chunk.x == top_left.x)
                    c.add_wall(Vector2i(-1, 0));
                else if (current_chunk.x == bottom_right.x)
                    c.add_wall(Vector2i(1, 0));
                if (current_chunk.y == top_left.y)
                    c.add_wall(Vector2i(0, -1));
                else if (current_chunk.y == bottom_right.y)
                    c.add_wall(Vector2i(0, 1));
            }
        }
    }

    // Generate tunnels connecting rooms

    // OLD TUNNEL ALGORITHM
    // vector<Vec2> points = point_scatter(room_dist, scatter_tries);
    // // Create a tunnel for each point
    // for (Vec2 p : points)
    // {
    //     // Find the tunnel's end point - attempts scatter_tries times to find a valid, inbounds end point
    //     // If no point is found the tunnel will simply not generate.
    //     Vector2i end_chunk_pos;
    //     bool success{false};
    //     for (int i{0}; i < scatter_tries; i++)
    //     {
    //         double tunnel_angle{(double)main_rng.next() / UINT_MAX * 2 * PI};
    //         double tunnel_len{(double)main_rng.next() / UINT_MAX * (room_size_max - room_size_min) + room_size_min};
    //         end_chunk_pos = Vector2i((int)((p.x + (tunnel_len * cos(tunnel_angle))) / chunk_size.x), (int)((p.y + (tunnel_len * sin(tunnel_angle))) / chunk_size.y));
    //         if (end_chunk_pos.x < 0 || end_chunk_pos.x >= chunks.size() || end_chunk_pos.y < 0 || end_chunk_pos.y >= chunks[0].size())
    //         {
    //             continue;
    //         }
    //         else
    //         {
    //             success = true;
    //             break;
    //         }
    //     }
    //     if (!success)
    //     {
    //         continue;
    //     }

    //     // Create a chain of chunks in a tunnel formation
    //     Vector2i start_chunk_pos{Vector2i((int)(p.x / chunk_size.x), (int)(p.y / chunk_size.y))};
    //     // ERR_FAIL_COND_MSG(start_chunk_pos.x < 0 || start_chunk_pos.x >= chunks.size() || start_chunk_pos.y < 0 || start_chunk_pos.y >= chunks[0].size(), "oob start chunk");
    //     // ERR_FAIL_COND_MSG(end_chunk_pos.x < 0 || end_chunk_pos.x >= chunks.size() || end_chunk_pos.y < 0 || end_chunk_pos.y >= chunks[0].size(), "oob end chunk");
    //     Vector2i distance_to_end{end_chunk_pos - start_chunk_pos};
    //     // (x > 0) - (x < 0) returns 1 if x is positive and -1 if x is negative. https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
    //     // Just subtraction using booleans which are auto casted to 1 or 0, not as scary as it looks
    //     Vector2i direction{Vector2i((distance_to_end.x > 0) - (distance_to_end.x < 0), (distance_to_end.y > 0) - (distance_to_end.y < 0))};
    //     distance_to_end.x = abs(distance_to_end.x);
    //     distance_to_end.y = abs(distance_to_end.y);

    //     // Declaring variables to be used within the following while loop
    //     Vector2i current_chunk_pos{start_chunk_pos};
    //     // Set the entry direction into the tunnel to the opposite y or x direction at random
    //     bool rand_entry{main_rng.next() % 2 == 0};
    //     Vector2i entry_direction;
    //     if (rand_entry)
    //         entry_direction = Vector2i(-direction.x, 0);
    //     else
    //         entry_direction = Vector2i(0, -direction.y);
    //     Vector2i next_chunk_pos;

    //     // prints info about each tunnel
    //     // godot::_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/function failed.",
    //     //                         "tunnel data, start: " + start_chunk_pos + ", end: " + end_chunk_pos + ", dir: " + direction);

    //     // Randomly pick a direction to snake towards the end point, until close enough that a straight line must be taken to reach the end
    //     while (distance_to_end.x > 0 && distance_to_end.y > 0)
    //     {
    //         // random 1 in 2
    //         bool rand_dir{main_rng.next() % 2 == 0};
    //         // move in x dir
    //         if (rand_dir)
    //         {
    //             next_chunk_pos = current_chunk_pos + Vector2i(direction.x, 0);
    //             distance_to_end.x--;
    //         }
    //         // move in y dir
    //         else
    //         {
    //             next_chunk_pos = current_chunk_pos + Vector2i(0, direction.y);
    //             distance_to_end.y--;
    //         }

    //         // ERR_FAIL_COND_MSG(current_chunk_pos.x < 0 || current_chunk_pos.x >= chunks.size() || current_chunk_pos.y < 0 || current_chunk_pos.y >= chunks[0].size(),
    //         //                   "oob chunk (1) @ " + current_chunk_pos + ", start: " + start_chunk_pos + ", end: " + end_chunk_pos + ", dir: " + direction);
    //         chunks[current_chunk_pos.x][current_chunk_pos.y].add_exit(entry_direction);
    //         chunks[current_chunk_pos.x][current_chunk_pos.y].add_exit(next_chunk_pos - current_chunk_pos);
    //         // prints data about exits added to each chunk during this loop
    //         // godot::_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/function failed.",
    //         //                         "added entry dir: " + entry_direction + " added other exit: " + (next_chunk_pos - current_chunk_pos) +
    //         //                             ", tunnel data, start: " + start_chunk_pos + ", end: " + end_chunk_pos + ", dir: " + direction + " @ " + current_chunk_pos);
    //         entry_direction = current_chunk_pos - next_chunk_pos;
    //         current_chunk_pos = next_chunk_pos;
    //     }
    //     // Finish the last chunk in the loop
    //     chunks[current_chunk_pos.x][current_chunk_pos.y].add_exit(entry_direction);
    //     chunks[current_chunk_pos.x][current_chunk_pos.y].add_exit(distance_to_end.x > 0 ? Vector2i(direction.x, 0) : Vector2i(0, direction.y));

    //     // Close any remaining distance (hardcoding this is just easier, less calculations needed)
    //     while (distance_to_end.x > 1)
    //     {
    //         current_chunk_pos = current_chunk_pos + Vector2i(direction.x, 0);
    //         // ERR_FAIL_COND_MSG(current_chunk_pos.x < 0 || current_chunk_pos.x >= chunks.size() || current_chunk_pos.y < 0 || current_chunk_pos.y >= chunks[0].size(),
    //         //                   "oob chunk (2) @ " + current_chunk_pos + ", start: " + start_chunk_pos + ", end: " + end_chunk_pos + ", dir: " + direction);
    //         chunks[current_chunk_pos.x][current_chunk_pos.y].add_exit(Vector2i(-direction.x, 0));
    //         chunks[current_chunk_pos.x][current_chunk_pos.y].add_exit(Vector2i(direction.x, 0));
    //         distance_to_end.x--;
    //     }
    //     while (distance_to_end.y > 1)
    //     {
    //         current_chunk_pos = current_chunk_pos + Vector2i(0, direction.y);
    //         // ERR_FAIL_COND_MSG(current_chunk_pos.x < 0 || current_chunk_pos.x >= chunks.size() || current_chunk_pos.y < 0 || current_chunk_pos.y >= chunks[0].size(),
    //         //                   "oob chunk (3)" + current_chunk_pos + " start: " + start_chunk_pos + " end: " + end_chunk_pos);
    //         chunks[current_chunk_pos.x][current_chunk_pos.y].add_exit(Vector2i(0, -direction.y));
    //         chunks[current_chunk_pos.x][current_chunk_pos.y].add_exit(Vector2i(0, direction.y));
    //         distance_to_end.y--;
    //     }

    //     // Finally, set the last chunk
    //     chunks[end_chunk_pos.x][end_chunk_pos.y].add_exit(current_chunk_pos - end_chunk_pos);
    //     bool rand_exit{main_rng.next() % 2 == 0};
    //     // exit x
    //     if (rand_exit)
    //     {
    //         chunks[end_chunk_pos.x][end_chunk_pos.y].add_exit(Vector2i(direction.x, 0));
    //     }
    //     // exit y
    //     else
    //     {
    //         chunks[end_chunk_pos.x][end_chunk_pos.y].add_exit(Vector2i(0, direction.y));
    //     }
    // }

    // Generate chunks
    for (int x{0}; x < chunks.size(); x++)
    {
        for (int y{0}; y < chunks[0].size(); y++)
        {
            chunks[x][y].generate(map, x, y, this);
        }
    }

    // TODO: Compile together godot-typed arrays of objects from chunk data
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
void TerrainGenerator::object_scatter(double r, int k)
{
    const double cell_size{r / sqrt(2)};
    const int cells_x{(int)ceil(size.x / cell_size) + 1};
    const int cells_y{(int)ceil(size.y / cell_size) + 1};

    // might be unneeded, we'll see how output works best
    // initialize to -1, 0 vectors
    vector<vector<Vec2>> grid;
    for (int x{0}; x < cells_x; ++x)
    {
        vector<Vec2> v;
        for (int y{0}; y < cells_y; ++y)
        {
            v.push_back(Vec2{-1.0, 0.0});
        }
        grid.push_back(v);
    }

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

// Similar to object_scatter but instead returns a list of points
// Basically copied and pasted, somewhat inefficient but whatever
vector<Vec2> TerrainGenerator::point_scatter(double r, int k)
{
    const double cell_size{r / sqrt(2)};
    const int cells_x{(int)ceil(size.x / cell_size) + 1};
    const int cells_y{(int)ceil(size.y / cell_size) + 1};

    // might be unneeded, we'll see how output works best
    // initialize to -1, 0 vectors
    vector<vector<Vec2>> grid;
    for (int x{0}; x < cells_x; ++x)
    {
        vector<Vec2> v;
        for (int y{0}; y < cells_y; ++y)
        {
            v.push_back(Vec2{-1.0, 0.0});
        }
        grid.push_back(v);
    }

    // Initial point
    vector<Vec2> active;
    vector<Vec2> points;
    Vec2 p0{(double)(main_rng.next() % size.x), (double)(main_rng.next() % size.y)};
    points.push_back(p0);
    grid[(int)p0.x / cell_size][(int)p0.y / cell_size] = p0;
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
            points.push_back(new_point);
            grid[(int)new_point.x / cell_size][(int)new_point.y / cell_size] = new_point;
            success = true;
            break;
        }

        // Add new point or remove p from active
        if (!success)
        {
            active.erase(active.begin() + idx);
        }
    }

    return points;
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

TerrainRNG TerrainGenerator::get_main_rng() const
{
    return main_rng;
}

void TerrainGenerator::set_max_removed_objects(const int m)
{
    max_removed_objects = m;
}

int TerrainGenerator::get_max_removed_objects() const
{
    return max_removed_objects;
}

void TerrainGenerator::set_room_dist(const double d)
{
    room_dist = d;
}

double TerrainGenerator::get_room_dist() const
{
    return room_dist;
}

void TerrainGenerator::set_room_size_max(const Vector2i s)
{
    room_size_max = s;
}

Vector2i TerrainGenerator::get_room_size_max() const
{
    return room_size_max;
}

void TerrainGenerator::set_room_size_min(const Vector2i s)
{
    room_size_min = s;
}

Vector2i TerrainGenerator::get_room_size_min() const
{
    return room_size_min;
}