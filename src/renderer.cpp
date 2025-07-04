#include "renderer.h"

Renderer::Renderer() : shader(nullptr)
{
}

Renderer::~Renderer()
{
    delete shader;
}