#include "World.h"

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
        float x = (rand() % 51) - 25;
        float z = (rand() % 50);
        GameObject* obj = new GameObject(model, glm::vec3(x, 0.0f, z));
        AddObject(obj);
    }
}

std::vector<GameObject*> const World::GetObjects()
{
    return Objects;
}
