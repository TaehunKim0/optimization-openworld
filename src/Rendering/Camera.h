#pragma once 
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera(const glm::vec3& InPosition = glm::vec3(0.0f, 0.0f, 3.0f))
        : Position(InPosition)
        , Front(glm::vec3(0.0f, 0.0f, -1.0f))
        , WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
        , Yaw(-90.0f)
        , Pitch(0.0f)
        , MovementSpeed(2.5f)
        , MouseSensitivity(0.1f)
    {
        UpdateCameraVectors();
    }

    glm::mat4 GetViewMatrix() const;
    void ProcessKeyboard(const std::string& Direction, float DeltaTime);
    void ProcessMouseMovement(float XOffset, float YOffset);

private:
    void UpdateCameraVectors();

public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
};

