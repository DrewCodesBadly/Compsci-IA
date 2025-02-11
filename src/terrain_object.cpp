#include "terrain_object.h"

TerrainObject::TerrainObject(godot::Vector2i p, double s, int pass_idx)
    : pos{p}, size{s}, pass_index{pass_idx}
{
}

TerrainObject::~TerrainObject() {}

godot::Vector2i TerrainObject::get_pos() const
{
    return pos;
}

int TerrainObject::get_pass_index() const
{
    return pass_index;
}