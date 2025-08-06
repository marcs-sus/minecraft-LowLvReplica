#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace glm;

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

enum CameraSpeedAdjust
{
    SPEED_UP,
    SPEED_DOWN
};

// Camera default values
extern const float YAW;
extern const float PITCH;
extern const float SPEED;
extern const float SENSITIVITY;
extern const float ZOOM;

class Camera
{
public:
    // Camera attributes
    vec3 Position, Front, Up, Right, WorldUp;

    // Euler Angles
    float Yaw, Pitch;

    // Camera options
    float MovementSpeed, MouseSensitivity, Zoom;

    // Constructors
    Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f),
           vec3 up = vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW,
           float pitch = PITCH);

    Camera(float posX, float posY, float posZ,
           float upX, float upY, float upZ,
           float yaw, float pitch);

    // Destructor
    ~Camera() = default;

    // Public methods
    mat4 GetViewMatrix() const;
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessKeyboard(CameraSpeedAdjust adjustment);
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yOffset);

private:
    // Private methods
    void UpdateCameraVectors();
};
