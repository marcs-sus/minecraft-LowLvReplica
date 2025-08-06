#include "core/application.h"
#include <iostream>

// Temp includes
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "world/block.h"

#include "shader/shader.h"
#include "player/camera.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Paths
const char *vertexPath = "shaders/vertexShader.vs";
const char *fragmentPath = "shaders/fragmentShader.fs";

const char *grassBlockPath = "assets/textures/grass_block_atlas.png";
const char *stoneBlockPath = "assets/textures/stone_block.png";

// Camera (temp, move to player class)
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Temp shader and block
Shader *shader = nullptr;
Block *block = nullptr;

Application::Application(unsigned int screenWidth, unsigned int screenHeight, const char *title)
    : screenWidth(screenWidth), screenHeight(screenHeight), deltaTime(0.0f), lastFrame(0.0f),
      renderer(), world(16, 16, 16), camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
    InitGLFW();
    window = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    InitGLAD();

    glEnable(GL_DEPTH_TEST);

    // Temp shader
    shader = new Shader(vertexPath, fragmentPath);

    // Temp block
    block = new Block(BLOCK_GRASS, grassBlockPath);

    // Use shader
    shader->Use();
    shader->SetInt("grassBlockTexture", 0);
}

Application::~Application()
{
    // Cleanup
    delete shader;

    // TODO ~Block();

    // Terminate GLFW
    glfwTerminate();
}

void Application::Run()
{
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Main process
        ProcessInput();
        Update();
        Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::ProcessInput()
{
    // Esc - Exit application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // W, A, S, D, Space and Left Shift - Move camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    // Q and E - Camera speed adjustment
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(SPEED_UP);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(SPEED_DOWN);
}

void Application::Update()
{
    // TODO
}

void Application::Render()
{
    // Clear screen
    glClearColor(0.47f, 0.65f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, block->texture);

    shader->Use();

    // Matrices
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    shader->SetMat4("model", model);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader->SetMat4("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    shader->SetMat4("view", view);

    // Draw
    glBindVertexArray(block->VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Application::InitGLFW()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::InitGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}

void Application::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::MouseCallback(GLFWwindow *window, double xPosIn, double yPosIn)
{
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app)
    {
        float xPos = static_cast<float>(xPosIn);
        float yPos = static_cast<float>(yPosIn);

        if (firstMouse)
        {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        float xOffset = xPos - lastX;
        float yOffset = lastY - yPos;

        lastX = xPos;
        lastY = yPos;

        app->camera.ProcessMouseMovement(xOffset, yOffset);
    }
}

void Application::ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app)
    {
        app->camera.ProcessMouseScroll(static_cast<float>(yOffset));
    }
}
