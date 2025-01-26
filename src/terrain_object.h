#ifndef TERRAIN_OBJECT_H
#define TERRAIN_OBJECT_H

#include <godot_cpp/classes/node.hpp>

class TerrainObject
{
private:
    godot::Vector2i pos;
    double size;

public:
    TerrainObject(godot::Vector2i pos, double size);
    ~TerrainObject();
    godot::Vector2i get_pos() const;
};
#endif