#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "world.h"
#include "player.h"

class Application
{
public:
    // Constructor and Destructor
    Application(unsigned int screenWidth, unsigned int screenHeight, const char *title);
    ~Application();

    // Main Loop
    void Run();

private:
    // Window
    GLFWwindow *window;
    unsigned int screenWidth, screenHeight;

    // Objects
    Renderer renderer;
    World world;
    Player player;

    // Time management
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Callbacks
    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
    static void MouseCallback(GLFWwindow *window, double xPos, double yPos);
    static void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);

    // Functions
    void ProcessInput();
    void Update();
    void Render();
    void InitGLFW();
    void InitGLAD();
};

#endif