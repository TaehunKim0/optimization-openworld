#pragma once
#include <vector>
#include "../GameObject/GameObject.h"

class World 
{
public:
    ~World();

public:
    void AddObject(GameObject* obj);
    void Render(Shader& shader);
    void SpawnRandomModels(Model* model, int count);

    std::vector<GameObject*> const GetObjects();

private:
    std::vector<GameObject*> Objects;
};