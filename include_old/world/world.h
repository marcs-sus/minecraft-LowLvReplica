#ifndef WORLD_H
#define WORLD_H

#include <glm.hpp>

#include "block.h"

#include <vector>

class World
{
public:
    // Constructor and Destructor
    World(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ);
    ~World();

    // Functions
    void GenerateFlatWorld();
    BlockType GetBlock(unsigned int x, unsigned int y, unsigned int z) const;
    void SetBlock(unsigned int x, unsigned int y, unsigned int z, Block block);

    // Rendering iterations
    inline unsigned int GetSizeX() const { return sizeX; }
    inline unsigned int GetSizeY() const { return sizeY; }
    inline unsigned int GetSizeZ() const { return sizeZ; }

private:
    // World data
    unsigned int sizeX, sizeY, sizeZ;
    std::vector<Block> blocks;

    // Helper to convert 3D coords to 1D
    unsigned int GetIndex(unsigned int x, unsigned int y, unsigned int z) const;
};

#endif