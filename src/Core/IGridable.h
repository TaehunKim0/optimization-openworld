#pragma once
#include <glm/glm.hpp>

class IGridable 
{
public:
    virtual ~IGridable() = default;

    virtual glm::vec3 GetPosition() const = 0;
    virtual float GetRadius() const = 0;

    virtual int GetGridIndex() const = 0;
    virtual void SetGridIndex(int InIndex) = 0;
};