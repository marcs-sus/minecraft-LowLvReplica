#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm.hpp>

#include "shader/shader.h"

class World;

class Renderer
{
public:
    // Constructor and Destructor
    Renderer();
    ~Renderer();

    // Functions
    void Init();
    void DrawWorld(const World &world, const glm::mat4 &view, const glm::mat4 &projection);
    void SetClearColor(float r, float g, float b, float a);

private:
    Shader *shader;
    unsigned int VBO, VAO, EBO;
    unsigned int texture;

    void SetupRendering();
    void LoadTextures();
};

#endif