#ifndef PLAYER_H
#define PLAYER_H

#include "camera.h"
#include "world.h"

class Player : public Camera // Extend camera for now
{
public:
    // Constructor
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);

    // Override keyboard input process
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessKeyboard(CameraSpeedAdjust adjustment);

    // Block interaction
    void BreakBlock(World &world);
    void PlaceBlock(World &world, BlockType type);
};

#endif