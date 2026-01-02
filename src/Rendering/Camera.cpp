#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "Camera.h"

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(const std::string& Direction, float DeltaTime)
{
    const float Velocity = MovementSpeed * DeltaTime;

    if (Direction == "FORWARD")  Position += Front * Velocity;
    if (Direction == "BACKWARD") Position -= Front * Velocity;
    if (Direction == "LEFT")     Position -= Right * Velocity;
    if (Direction == "RIGHT")    Position += Right * Velocity;
    if (Direction == "UP")       Position += Up * Velocity;
    if (Direction == "DOWN")     Position -= Up * Velocity;
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 NewFront;
    NewFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    NewFront.y = sin(glm::radians(Pitch));
    NewFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(NewFront);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}