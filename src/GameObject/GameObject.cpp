#include "GameObject.h"

#include "../Mesh/Model.h"
#include "../Rendering/Shader.h"

GameObject::GameObject(Model* InModel, glm::vec3 InPos)
    : ModelPtr(InModel), Position(InPos), Rotation(0.0f), Scale(1.0f), CurrentGridIndex(-1), CollisionRadius(0.f)
{
}

void GameObject::Draw(Shader& InShader)
{
    glm::mat4 ModelMat = glm::mat4(1.0f);

    // 1. 이동(Translation)
    ModelMat = glm::translate(ModelMat, Position);

    // 2. 회전(Rotation) - X, Y, Z축 순으로 회전 적용
    ModelMat = glm::rotate(ModelMat, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
    ModelMat = glm::rotate(ModelMat, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
    ModelMat = glm::rotate(ModelMat, glm::radians(Rotation.z), glm::vec3(0, 0, 1));

    // 3. 크기(Scale)
    ModelMat = glm::scale(ModelMat, Scale);

    InShader.SetMat4("model", ModelMat);
    ModelPtr->Draw(InShader.ID);
}

glm::vec3 GameObject::GetPosition() const
{
    return Position;
}

float GameObject::GetRadius() const
{
    return CollisionRadius;
}

int GameObject::GetGridIndex() const
{
    return CurrentGridIndex;
}

void GameObject::SetGridIndex(int InIndex)
{
    CurrentGridIndex = InIndex;
}
