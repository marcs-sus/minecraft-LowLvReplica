#include "world/block.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

// Default block vertices and indices
float Block::vertices[] = {
    // Back face
    -0.5f, -0.5f, -0.5f, 0.333f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.667f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.667f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.333f, 1.0f,

    // Front face
    -0.5f, -0.5f, 0.5f, 0.333f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.667f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.667f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.333f, 1.0f,

    // Left face
    -0.5f, 0.5f, 0.5f, 0.333f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.667f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.667f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.333f, 0.0f,

    // Right face
    0.5f, 0.5f, 0.5f, 0.667f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.333f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.333f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.667f, 0.0f,

    // Bottom face
    -0.5f, -0.5f, -0.5f, 0.667f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.667f, 0.0f,

    // Top face
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.333f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.333f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f};

unsigned int Block::indices[] = {
    0, 1, 2, 2, 3, 0,       // Back face
    4, 5, 6, 6, 7, 4,       // Front face
    8, 9, 10, 10, 11, 8,    // Left face
    12, 13, 14, 14, 15, 12, // Right face
    16, 17, 18, 18, 19, 16, // Bottom face
    20, 21, 22, 22, 23, 20  // Top face
};

Block::Block(BlockType type, const char *texturePath)
    : type(type), texture(texture), VAO(0), VBO(0), EBO(0)
{
    // Setup VAO, VBO and EBO
    SetupObjects();

    // Load textures
    LoadTextures(texturePath);
}

Block::~Block()
{
    // Cleanup vertex array objects
    glDeleteVertexArrays(1, &VAO);

    // Cleanup buffers
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Cleanup textures
    glDeleteTextures(1, &texture);
}

void Block::SetupObjects()
{
    // Setup VAO, VBO and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Set texture attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Unbind objects
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Block::LoadTextures(const char *texturePath)
{
    // Generate textures
    glGenTextures(1, &texture);

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Use stb to flip image
    stbi_set_flip_vertically_on_load(true);

    // Use stb to load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    // Generate texture
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Free image data
    stbi_image_free(data);
}
