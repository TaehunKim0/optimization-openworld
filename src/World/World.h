#pragma once
#include <vector>

class GameObject;
class Shader;
class Model;
class SpatialHashGrid;


class World 
{
public:
    ~World();

public:
    void AddObject(GameObject* obj);
    void Render(Shader& shader);
    void SpawnRandomModels(Model* model, int count);

    void InitGrid(float width, float height, float cellSize);
    void RenderGrid(Shader& debugShader);

    const std::vector<GameObject*>& GetObjects() const { return Objects; } // 복사 일어나지 않게

private:
    std::vector<GameObject*> Objects;

private:
    unsigned int GridVAO, GridVBO;
    int GridLineCount;
    const int MaxSpawnX = 200;
    const int MaxSpawnZ = 200;
};