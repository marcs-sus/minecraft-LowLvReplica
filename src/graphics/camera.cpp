#include "graphics/camera.h"

using namespace glm;

// Camera default values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// Constructor with vectors
Camera::Camera(vec3 position, vec3 up, float yaw, float pitch)
    : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM), Position(position),
      WorldUp(up), Yaw(yaw), Pitch(pitch)
{
    UpdateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ,
               float upX, float upY, float upZ,
               float yaw, float pitch)
    : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM),
      Position(vec3(posX, posY, posZ)),
      WorldUp(vec3(upX, upY, upZ)), Yaw(yaw), Pitch(pitch)
{
    UpdateCameraVectors();
}

// Destructor
Camera::~Camera() = default;

// Calculate the view matrix
mat4 Camera::GetViewMatrix() const
{
    return lookAt(Position, Position + Front, Up);
}

// Process keyboard input for camera movement
void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    const float velocity = MovementSpeed * deltaTime;

    switch (direction)
    {
    case FORWARD:
        Position += Front * velocity;
        break;
    case BACKWARD:
        Position -= Front * velocity;
        break;
    case LEFT:
        Position -= Right * velocity;
        break;
    case RIGHT:
        Position += Right * velocity;
        break;
    case UP:
        Position += Up * velocity;
        break;
    case DOWN:
        Position -= Up * velocity;
        break;
    }
}

// Process keyboard input to adjust camera speed
void Camera::ProcessKeyboard(CameraSpeedAdjust adjustment)
{
    constexpr float SPEED_INCREMENT = 0.1f;
    constexpr float MAX_SPEED = 35.0f;
    constexpr float MIN_SPEED = 0.5f;

    switch (adjustment)
    {
    case SPEED_UP:
        if (MovementSpeed < MAX_SPEED)
            MovementSpeed += SPEED_INCREMENT;
        break;
    case SPEED_DOWN:
        if (MovementSpeed > MIN_SPEED)
            MovementSpeed -= SPEED_INCREMENT;
        break;
    }
}

// Process mouse movement for camera orientation
void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    if (constrainPitch)
    {
        constexpr float PITCH_LIMIT = 89.0f;

        if (Pitch > PITCH_LIMIT)
            Pitch = PITCH_LIMIT;
        if (Pitch < -PITCH_LIMIT)
            Pitch = -PITCH_LIMIT;
    }

    UpdateCameraVectors();
}

// Process mouse scroll for zooming
void Camera::ProcessMouseScroll(float yOffset)
{
    constexpr float MIN_ZOOM = 1.0f;
    constexpr float MAX_ZOOM = 45.0f;

    Zoom -= yOffset;

    if (Zoom < MIN_ZOOM)
        Zoom = MIN_ZOOM;
    if (Zoom > MAX_ZOOM)
        Zoom = MAX_ZOOM;
}

// Calculates Front vector from Yaw and Pitch angles
void Camera::UpdateCameraVectors()
{
    vec3 front;
    front.x = cos(radians(Yaw)) * cos(radians(Pitch));
    front.y = sin(radians(Pitch));
    front.z = sin(radians(Yaw)) * cos(radians(Pitch));

    Front = normalize(front);
    Right = normalize(cross(Front, WorldUp));
    Up = normalize(cross(Right, Front));
}
