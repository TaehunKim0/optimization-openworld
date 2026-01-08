#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "Camera.h"

void Camera::UpdateCameraInput(GLFWwindow* InWindow, float InDeltaTime)
{
    if (glfwGetKey(InWindow, GLFW_KEY_W) == GLFW_PRESS) ProcessKeyboard("FORWARD", InDeltaTime);
    if (glfwGetKey(InWindow, GLFW_KEY_S) == GLFW_PRESS) ProcessKeyboard("BACKWARD", InDeltaTime);
    if (glfwGetKey(InWindow, GLFW_KEY_A) == GLFW_PRESS) ProcessKeyboard("LEFT", InDeltaTime);
    if (glfwGetKey(InWindow, GLFW_KEY_D) == GLFW_PRESS) ProcessKeyboard("RIGHT", InDeltaTime);
    if (glfwGetKey(InWindow, GLFW_KEY_SPACE) == GLFW_PRESS) ProcessKeyboard("UP", InDeltaTime);
    if (glfwGetKey(InWindow, GLFW_KEY_Q) == GLFW_PRESS) ProcessKeyboard("DOWN", InDeltaTime);
}

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

void Camera::ProcessMouseMovement(float XOffset, float YOffset)
{
    XOffset *= MouseSensitivity;
    YOffset *= MouseSensitivity;

    Yaw += XOffset;
    Pitch += YOffset;

    if (Pitch > 89.0f)  Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    UpdateCameraVectors();
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