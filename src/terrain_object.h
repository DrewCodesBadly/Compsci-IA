#ifndef TERRAIN_OBJECT_H
#define TERRAIN_OBJECT_H
#include "vec2.h"
class TerrainObject
{
private:
    Vec2 pos;
    double size;

public:
    TerrainObject(Vec2 pos, double size);
    ~TerrainObject();
};
#endif