#include "chunk_data.h"

using namespace godot;

void ChunkData::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("is_tunnel"), &ChunkData::is_tunnel);
    ClassDB::bind_method(D_METHOD("set_tunnel", "t"), &ChunkData::set_tunnel);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tunnel"), "set_tunnel", "is_tunnel");

    ClassDB::bind_method(D_METHOD("is_out_of_bounds"), &ChunkData::is_out_of_bounds);
    ClassDB::bind_method(D_METHOD("set_out_of_bounds", "oob"), &ChunkData::set_out_of_bounds);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "out_of_bounds"), "set_out_of_bounds", "is_out_of_bounds");

    ClassDB::bind_method(D_METHOD("get_wall_sides"), &ChunkData::get_wall_sides);
    ClassDB::bind_method(D_METHOD("set_wall_sides", "sides"), &ChunkData::set_wall_sides);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "wall_sides"), "set_wall_sides", "get_wall_sides");

    ClassDB::bind_method(D_METHOD("get_biome"), &ChunkData::get_biome);
    ClassDB::bind_method(D_METHOD("set_biome", "b"), &ChunkData::set_biome);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "biome"), "set_biome", "get_biome");
}

ChunkData::ChunkData()
    : wall_sides{Array()}, tunnel{false}, out_of_bounds{true}, biome{0}
{
}

ChunkData::~ChunkData()
{
}

bool ChunkData::is_tunnel() const
{
    return tunnel;
}

void ChunkData::set_tunnel(const bool t)
{
    tunnel = t;
}

bool ChunkData::is_out_of_bounds() const
{
    return out_of_bounds;
}

void ChunkData::set_out_of_bounds(const bool oob)
{
    out_of_bounds = oob;
}

Array ChunkData::get_wall_sides() const
{
    return wall_sides;
}

void ChunkData::set_wall_sides(const Array sides)
{
    wall_sides = sides;
}

int ChunkData::get_biome() const
{
    return biome;
}

void ChunkData::set_biome(const int b)
{
    biome = b;
}