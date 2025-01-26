#include "terrain_object.h"

TerrainObject::TerrainObject(godot::Vector2i p, double s)
    : pos{p}, size{s}
{
}

TerrainObject::~TerrainObject() {}

godot::Vector2i TerrainObject::get_pos() const
{
    return pos;
}