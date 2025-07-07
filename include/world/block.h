#ifndef BLOCK_H
#define BLOCK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

enum BlockType
{
    BLOCK_AIR = 1,
    BLOCK_GRASS,
    BLOCK_STONE
};

class Block
{
public:
    // Variables
    BlockType type;
    static float vertices[];
    static unsigned int indices[];
    unsigned int VAO, VBO, EBO;
    unsigned int texture;

    // Constructor and Destructor
    Block(BlockType type, const char *texturePath);
    ~Block();

private:
    // Functions
    void SetupObjects();
    void LoadTextures(const char *texturePath);
};

#endif