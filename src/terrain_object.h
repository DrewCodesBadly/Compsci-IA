#ifndef TERRAIN_OBJECT_H
#define TERRAIN_OBJECT_H

#include <godot_cpp/classes/node.hpp>

class TerrainObject
{
private:
    godot::Vector2i pos;
    double size;
    int pass_index;

public:
    TerrainObject(godot::Vector2i pos, double size, int pass_idx);
    ~TerrainObject();
    godot::Vector2i get_pos() const;
    int get_pass_index() const;
};
#endif