#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Mesh/Model.h"
#include "../Rendering/Shader.h"

class GameObject
{
public:
    GameObject(Model* InModel, glm::vec3 InPos = glm::vec3(0.0f));
    void Draw(Shader& InShader);

public:
    void Translate(const glm::vec3& delta) { Position += delta; }
    void SetPosition(const glm::vec3& newPos) { Position = newPos; }
    void SetRotation(const glm::vec3& newRot) { Rotation = newRot;}
    void SetScale(const glm::vec3& newScale) {Scale = newScale;}

private:
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;

private:
    Model* ModelPtr;
};