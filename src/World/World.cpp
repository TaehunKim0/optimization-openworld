#include "World.h"
#include <glad/glad.h>
#include "../Rendering/Shader.h"
#include "../Core/SpatialHashGrid.h"
#include "../GameObject/GameObject.h"
#include "../Mesh/Model.h"

World::~World()
{
    for (auto obj : Objects) 
    {
        delete obj;
    }
    Objects.clear();
}

void World::AddObject(GameObject* obj)
{
    Objects.push_back(obj);
}

void World::Render(Shader& shader)
{
    for (auto obj : Objects) 
    {
        obj->Draw(shader);
    }
}

void World::SpawnRandomModels(Model* model, int count)
{
    for (int i = 0; i < count; i++)
    {
        float x = static_cast<float>(rand() % MaxSpawnX);
        float z = static_cast<float>(rand() % MaxSpawnZ);
        GameObject* obj = new GameObject(model, glm::vec3(x, 0.0f, z));
        AddObject(obj);
    }
}

void World::InitGrid(float width, float height, float cellSize) {
    std::vector<float> Vertices;

    // 가로선 (Z축을 따라 0에서 height까지)
    for (float i = 0; i <= width; i += cellSize) {
        Vertices.push_back(i); Vertices.push_back(0.0f); Vertices.push_back(0.0f);
        Vertices.push_back(i); Vertices.push_back(0.0f); Vertices.push_back(height);
    }

    // 세로선 (X축을 따라 0에서 width까지)
    for (float i = 0; i <= height; i += cellSize) {
        Vertices.push_back(0.0f); Vertices.push_back(0.0f); Vertices.push_back(i);
        Vertices.push_back(width); Vertices.push_back(0.0f); Vertices.push_back(i);
    }

    GridLineCount = (int)Vertices.size() / 3;

    glGenVertexArrays(1, &GridVAO);
    glGenBuffers(1, &GridVBO);

    glBindVertexArray(GridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, GridVBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void World::RenderGrid(Shader& debugShader) {
    glm::mat4 model = glm::mat4(1.0f);
    debugShader.SetMat4("model", model);

    glBindVertexArray(GridVAO);
    glDrawArrays(GL_LINES, 0, GridLineCount);
    glBindVertexArray(0);
}