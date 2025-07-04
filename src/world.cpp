#include "world.h"

World::World(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ)
    : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ), blocks(sizeX * sizeY * sizeZ)
{
}

World::~World()
{
}