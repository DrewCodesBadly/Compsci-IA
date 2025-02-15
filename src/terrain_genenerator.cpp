#include "terrain_generator.h"
#include "terrain_rng.h"
#include <godot_cpp/core/class_db.hpp>
#include <vector>
#include <cmath>
#include <godot_cpp/variant/utility_functions.hpp>

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

    // rooms
    ClassDB::bind_method(D_METHOD("get_room_distance"), &TerrainGenerator::get_room_dist);
    ClassDB::bind_method(D_METHOD("set_room_distance", "d"), &TerrainGenerator::set_room_dist);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "room_distance"), "set_room_distance", "get_room_distance");
    ClassDB::bind_method(D_METHOD("get_room_size_max"), &TerrainGenerator::get_room_size_max);
    ClassDB::bind_method(D_METHOD("set_room_size_max", "l"), &TerrainGenerator::set_room_size_max);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "room_size_max"), "set_room_size_max", "get_room_size_max");
    ClassDB::bind_method(D_METHOD("get_room_size_min"), &TerrainGenerator::get_room_size_min);
    ClassDB::bind_method(D_METHOD("set_room_size_min", "l"), &TerrainGenerator::set_room_size_min);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "room_size_min"), "set_room_size_min", "get_room_size_min");

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

    ClassDB::bind_method(D_METHOD("get_min_tunnels"), &TerrainGenerator::get_min_tunnels);
    ClassDB::bind_method(D_METHOD("set_min_tunnels", "t"), &TerrainGenerator::set_min_tunnels);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "min_tunnels"), "set_min_tunnels", "get_min_tunnels");

    ClassDB::bind_method(D_METHOD("get_max_tunnels"), &TerrainGenerator::get_max_tunnels);
    ClassDB::bind_method(D_METHOD("set_max_tunnels", "t"), &TerrainGenerator::set_max_tunnels);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_tunnels"), "set_max_tunnels", "get_max_tunnels");

    // ClassDB::bind_method(D_METHOD("test_noise", "v"), &TerrainGenerator::test_noise);

    ClassDB::bind_method(D_METHOD("world_to_nearest_chunk_coords", "world_coords"), &TerrainGenerator::world_to_nearest_chunk_coords);
    ClassDB::bind_method(D_METHOD("chunk_to_world_coords", "chunk_coords"), &TerrainGenerator::chunk_to_world_coords);

    // Accessors for use in godot
    ClassDB::bind_method(D_METHOD("get_first_scatter_pass_points"), &TerrainGenerator::get_first_scatter_pass_points);
    ClassDB::bind_method(D_METHOD("get_second_scatter_pass_points"), &TerrainGenerator::get_second_scatter_pass_points);
    ClassDB::bind_method(D_METHOD("get_chunks"), &TerrainGenerator::get_chunks);
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

// Used for room/tunnel generation
struct RoomBounds
{
    Vector2i top_left;
    Vector2i bottom_right;
    bool connected = false;
};

// Entrypoint for map generation
void TerrainGenerator::generate()
{
    // Checks for valid parameters
    ERR_FAIL_COND_MSG(room_size_min > room_size_max, "room_size_min must be greater than room_size_max!");
    ERR_FAIL_COND_MSG(scatter_tries <= 0, "Scatter tries must be greater than 0 for the scattering algorithm to work properly.");
    ERR_FAIL_COND_MSG(size.x <= 0 || size.y <= 0, "Map size must be > 0 in both axes for a valid map to be generated.");
    ERR_FAIL_COND_MSG(chunk_size.x <= 0 || chunk_size.y <= 0, "TerrainChunk size must be > 0 in both axes for a valid map to be generated.");
    ERR_FAIL_COND_MSG(room_size_max.x < room_size_min.x || room_size_max.y < room_size_min.y, "Max room size should be greater than min room size.");

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

    // Object scatter passes
    // Pass 1: scatter small objects
    // Passes also have a pass_idx argument which is stored in TerrainObject instances.
    // This system easily allows more passes to be added.
    object_scatter(small_object_radius, scatter_tries, 0);

    // Pass 2: scatter large objects
    // Should naturally draw over small objects, so behavior will look about normal
    object_scatter(large_object_radius, scatter_tries, 1);

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
    Vector2i max_min_room_diff{room_size_max - room_size_min + Vector2i(1, 1)};
    vector<vector<Vec2>> rooms_points_grid = point_scatter(room_dist, scatter_tries);

    // Generate room chunks and create a new 2D array to store room structs instead
    vector<vector<RoomBounds>> rooms_grid;
    for (int x{0}; x < rooms_points_grid.size(); x++)
    {
        vector<RoomBounds> v;
        for (int y{0}; y < rooms_points_grid[0].size(); y++)
        {
            Vec2 p{rooms_points_grid[x][y]};
            // Ignore empty cells
            if (p.x == -1.0)
            {
                v.push_back(RoomBounds{Vector2i(-1, -1), Vector2i(-1, -1)});
                continue;
            }

            Vector2i room_size{Vector2i((main_rng.next() % max_min_room_diff.x) + room_size_min.x, (main_rng.next() % max_min_room_diff.y) + room_size_min.y)};
            Vector2i top_left{world_to_nearest_chunk_coords(Vector2(p.x, p.y)) - (room_size / 2)}; // integer division is fine here
            if (top_left.x < 0)
                top_left.x = 0;
            if (top_left.y < 0)
                top_left.y = 0;
            Vector2i bottom_right{top_left + room_size};
            if (bottom_right.x > chunks.size())
                bottom_right.x = chunks.size();
            if (bottom_right.y > chunks[0].size())
                bottom_right.y = chunks[0].size();

            // Iterate through all chunks in the room, creating walls and flagging them as not empty (actually part of the map)
            for (int x{top_left.x}; x < bottom_right.x; x++)
            {
                for (int y{top_left.y}; y < bottom_right.y; y++)
                {
                    TerrainChunk &c = chunks[x][y];
                    c.set_non_empty();
                    if (x == top_left.x)
                        c.add_wall(Vector2i(-1, 0));
                    if (x == bottom_right.x - 1)
                        c.add_wall(Vector2i(1, 0));
                    if (y == top_left.y)
                        c.add_wall(Vector2i(0, -1));
                    if (y == bottom_right.y - 1)
                        c.add_wall(Vector2i(0, 1));
                }
            }

            // Push a new RoomBounds (could be replaced w/nullptr and references, I think storing structs is faster though...)
            v.push_back(RoomBounds{top_left, bottom_right});
        }
        rooms_grid.push_back(v);
    }

    // Generate tunnels connecting rooms
    for (int x{0}; x < rooms_grid.size(); x++)
    {
        for (int y{0}; y < rooms_grid[0].size(); y++)
        {
            RoomBounds r{rooms_grid[x][y]};
            // Ignore empty rooms
            if (r.top_left.x == -1.0)
            {
                continue;
            }

            // Pick the (max) number of tunnels this room can have
            vector<Vector2i> directions{{Vector2i(-1, -1), Vector2i(0, -1), Vector2i(1, -1),
                                         Vector2i(-1, 0), /*Vector2i(0, 0),*/ Vector2i(1, 0),
                                         Vector2i(-1, 1), Vector2i(0, 1), Vector2i(1, 1)}};
            unsigned int tunnels{min_tunnels + (main_rng.next() % (max_tunnels - min_tunnels + 1))};

            // Check different directions for adjacent rooms to connect to
            while (tunnels > 0 && directions.size() > 0)
            {
                // Randomly select a direction and try it
                long unsigned int idx{main_rng.next() % directions.size()};
                Vector2i dir{directions[idx]};
                directions.erase(directions.begin() + idx);

                // If there is no room to connect to, pass and keep going
                Vector2i new_point{x + dir.x, y + dir.y};
                if (new_point.x < 0 || new_point.y < 0 || new_point.x > rooms_grid.size() || new_point.y > rooms_grid[0].size())
                    continue;
                RoomBounds connecting_to{rooms_grid[new_point.x][new_point.y]};
                if (connecting_to.top_left.x == -1)
                    continue;

                // Randomly snake from this room's center (roughly) point to the next room, creating a tunnel
                Vector2i pos{(r.top_left + r.bottom_right) / 2};
                Vector2i target_pos{(connecting_to.top_left + connecting_to.bottom_right) / 2};
                Vector2i tunnel_dir{target_pos - pos};
                Vector2i limits;
                if (tunnel_dir.x > 0)
                {
                    tunnel_dir.x = 1;
                    limits.x = connecting_to.top_left.x;
                }
                else
                {
                    tunnel_dir.x = -1;
                    limits.x = connecting_to.bottom_right.x;
                }
                if (tunnel_dir.y > 0)
                {
                    tunnel_dir.y = 1;
                    limits.y = connecting_to.top_left.y;
                }
                else
                {
                    tunnel_dir.y = -1;
                    limits.y = connecting_to.bottom_right.y;
                }
                bool exited_room{false};
                bool finished{false};
                while (pos.x != limits.x && pos.y != limits.y)
                {
                    // Shouldn't be possible to get out of bounds since we target a room which is in bounds
                    if (main_rng.next() % 2 == 0)
                    {
                        pos.x += tunnel_dir.x;
                    }
                    else
                    {
                        pos.y += tunnel_dir.y;
                    }

                    TerrainChunk &new_chunk{chunks[pos.x][pos.y]};
                    if (!new_chunk.is_empty() && exited_room)
                    {
                        // We've landed in the next room (if rooms are touching this could be an issue...)
                        finished = true;
                        break;
                    }
                    else if (new_chunk.is_tunnel())
                    {
                        finished = true;
                        break;
                    }
                    else
                    {
                        if (new_chunk.is_empty() && !exited_room)
                        {
                            exited_room = true;
                        }
                        new_chunk.set_non_empty();
                    }
                }
                // Finish the tunnel by going straight toward the room if needed
                if (!finished)
                {
                    if (pos.x != limits.x)
                    {
                        while (pos.x != limits.x)
                        {
                            pos.x += tunnel_dir.x;
                            TerrainChunk &new_chunk{chunks[pos.x][pos.y]};
                            if (!new_chunk.is_empty() && exited_room)
                                break;
                            else if (new_chunk.is_empty() && !exited_room)
                                exited_room = true;
                            else if (new_chunk.is_tunnel())
                                break;
                            new_chunk.set_non_empty();
                            new_chunk.set_tunnel();
                        }
                    }
                    else if (pos.y != limits.y)
                    {
                        while (pos.y != limits.y)
                        {
                            pos.y += tunnel_dir.y;
                            TerrainChunk &new_chunk{chunks[pos.x][pos.y]};
                            if (!new_chunk.is_empty() && exited_room)
                            {
                                break;
                            }
                            new_chunk.set_non_empty();
                        }
                    }
                }

                tunnels--;
            }
        }
    }

    // Create godot-readable data from generation data
    // Initializing new blank arrays
    first_scatter_pass_points = Array();
    second_scatter_pass_points = Array();
    chunks_gd = Array();
    for (int x{0}; x < chunks.size(); x++)
    {
        Array arr{Array()};
        chunks_gd.append(arr);
        for (int y{0}; y < chunks[0].size(); y++)
        {
            TerrainChunk &c = chunks[x][y];

            // Avoid adding anything more than a placeholder if the chunk is empty
            if (c.is_empty())
            {
                arr.append(Ref<ChunkData>(memnew(ChunkData())));
                continue;
            }

            Ref<ChunkData> data_ref = Ref<ChunkData>(memnew(ChunkData()));
            data_ref->set_out_of_bounds(false);
            data_ref->set_tunnel(c.is_tunnel());
            data_ref->set_wall_sides(c.get_wall_directions());
            data_ref->set_biome(c.get_biome_godot_compat());
            arr.append(data_ref);

            // Skip adding objects in tunnels (We don't want to block them!)]
            if (c.is_tunnel())
                continue;

            // Add objects to correct arrays
            for (TerrainObject o : c.get_objects())
            {
                if (o.get_pass_index() == 0)
                    first_scatter_pass_points.append(o.get_pos());
                else
                    second_scatter_pass_points.append(o.get_pos());
            }
        }
    }

    // Clear unneeded chunk data
    chunks.clear();
}

// used in object_scatter
void TerrainGenerator::insert_object(vector<vector<Vec2>> &grid, Vec2 p, double cellsize, double obj_size, int pass_idx)
{
    const int pos_x{(int)(p.x / cellsize)};
    const int pos_y{(int)(p.y / cellsize)};
    grid[pos_x][pos_y] = p;

    // Add object to appropriate chunk
    // Integer division takes the floor by default
    // Note that when object positions go from doubles to truncated ints some precision is lost, but it should be good enough
    chunks[p.x / chunk_size.x][p.y / chunk_size.y].add_object(TerrainObject(Vector2i(p.x, p.y), obj_size, pass_idx));
}

// Add variable object sizes in the future?
/* Uses the poisson disk sampling algorithm, more information can be found at:
    - https://sighack.com/post/poisson-disk-sampling-bridsons-algorithm
*/
// k is number of attempts before the algorithm gives up
// Note that the radius used will be double the object size
void TerrainGenerator::object_scatter(double r, int k, int pass_idx)
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
    insert_object(grid, p0, cell_size, r / 2.0, pass_idx);
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
            insert_object(grid, new_point, cell_size, r / 2.0, pass_idx);
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

// Similar to object_scatter but instead writes to a list of points and returns the grid
// Basically copied and pasted, somewhat inefficient but whatever
vector<vector<Vec2>> TerrainGenerator::point_scatter(double r, int k)
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

    return grid;
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

Array TerrainGenerator::get_first_scatter_pass_points()
{
    return first_scatter_pass_points;
}
Array TerrainGenerator::get_second_scatter_pass_points()
{
    return second_scatter_pass_points;
}
Array TerrainGenerator::get_chunks()
{
    return chunks_gd;
}

void TerrainGenerator::set_min_tunnels(const int t)
{
    if (t >= 0)
        min_tunnels = t;
}

void TerrainGenerator::set_max_tunnels(const int t)
{
    if (t >= 0)
        max_tunnels = t;
}

int TerrainGenerator::get_min_tunnels() const
{
    return min_tunnels;
}

int TerrainGenerator::get_max_tunnels() const
{
    return max_tunnels;
}