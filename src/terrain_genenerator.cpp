#include "terrain_generator.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void TerrainGenerator::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_seed"), &TerrainGenerator::get_seed);
    ClassDB::bind_method(D_METHOD("set_seed", "given_seed"), &TerrainGenerator::set_seed);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "seed"), "set_seed", "get_seed");
}

TerrainGenerator::TerrainGenerator(int given_seed)
{
    // Variable initialization goes here
    seed = given_seed;
}

TerrainGenerator::~TerrainGenerator()
{
    // Any needed cleanup here
}

void TerrainGenerator::set_seed(const int given_seed)
{
    seed = given_seed;
}

int TerrainGenerator::get_seed() const
{
    return seed;
}

// TODO: ADD
void TerrainGenerator::generate()
{
}