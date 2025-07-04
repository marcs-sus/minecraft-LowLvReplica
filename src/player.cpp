#include "player.h"

Player::Player(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Camera(position, up, yaw, pitch)
{
}

void Player::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    Camera::ProcessKeyboard(direction, deltaTime);
}

void Player::ProcessKeyboard(CameraSpeedAdjust adjustment)
{
    Camera::ProcessKeyboard(adjustment);
}
