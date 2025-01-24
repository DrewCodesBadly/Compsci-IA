#include "chunk.h"

Chunk::Chunk()
{
}

Chunk::~Chunk()
{
}

void Chunk::add_object(TerrainObject o)
{
    objects.push_back(o);
}