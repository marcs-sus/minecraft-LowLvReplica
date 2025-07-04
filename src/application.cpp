#include "application.h"
#include <iostream>

// Temp includes
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <shader.h>
#include <camera.h>

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

// Temp VAO, VBO, EBO, shader and texture
unsigned int VAO, VBO, EBO, texture;
Shader *shader = nullptr;

Application::Application(unsigned int screenWidth, unsigned int screenHeight, const char *title)
    : screenWidth(screenWidth), screenHeight(screenHeight), deltaTime(0.0f), lastFrame(0.0f),
      renderer(), world(16, 16, 16), player(glm::vec3(0.0f, 0.0f, 3.0f))
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

    // Temp vertices, move to Block class
    float vertices[] = {
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

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,       // Back face
        4, 5, 6, 6, 7, 4,       // Front face
        8, 9, 10, 10, 11, 8,    // Left face
        12, 13, 14, 14, 15, 12, // Right face
        16, 17, 18, 18, 19, 16, // Bottom face
        20, 21, 22, 22, 23, 20  // Top face
    };

    // VAO, VBO and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture
    int width, height, nrChannels;
    unsigned char *data;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(grassBlockPath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load grass_block texture" << std::endl;
    }

    stbi_image_free(data);

    // Use shader
    shader->Use();
    shader->SetInt("grassBlockTexture", 0);
}

Application::~Application()
{
    // Cleanup
    delete shader;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
    glBindTexture(GL_TEXTURE_2D, texture);

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
    glBindVertexArray(VAO);
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

        camera.ProcessMouseMovement(xOffset, yOffset);
    }
}

void Application::ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app)
    {
        camera.ProcessMouseScroll(static_cast<float>(yOffset));
    }
}
