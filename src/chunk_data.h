#ifndef CHUNKDATA_H
#define CHUNKDATA_H

#include <godot_cpp/classes/resource.hpp>
// #include <godot_cpp/classes/node.hpp>
#include <vector>

namespace godot
{
    // This class could really be a struct in other situations
    // Just an aggregate of data about a chunk that can be stored efficiently in the engine and accessed by the user.
    class ChunkData : public Resource
    {
        GDCLASS(ChunkData, Resource)

    private:
        Array wall_sides;
        bool tunnel;
        bool out_of_bounds;
        int biome;

    protected:
        static void _bind_methods();

    public:
        ChunkData();
        ~ChunkData();
        bool is_tunnel() const;
        void set_tunnel(const bool t);
        bool is_out_of_bounds() const;
        void set_out_of_bounds(const bool oob);
        Array get_wall_sides() const;
        void set_wall_sides(const Array sides);
        int get_biome() const;
        void set_biome(const int b);
    };
}

#endif