#include "SpatialHashGrid.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include "../GameObject/GameObject.h"
#include "IGridable.h"

SpatialHashGrid::SpatialHashGrid(float InWidth, float InHeight, float InCellSize)
    : CellSize(InCellSize)
{
    Cols = static_cast<int>(InWidth / InCellSize);
    Rows = static_cast<int>(InHeight / InCellSize);
    Cells.resize(Rows * Cols);
}

// ÁÂÇ¥¸¦ ÀÎµ¦½º·Î º¯È¯ O(1)
int SpatialHashGrid::GetCellIndex(float InX, float InZ)
{
    int Ix = static_cast<int>(InX / CellSize);
    int Iz = static_cast<int>(InZ / CellSize);

    Ix = std::clamp(Ix, 0, Cols - 1);
    Iz = std::clamp(Iz, 0, Rows - 1);

    return Iz * Cols + Ix;
}

const std::vector<IGridable*>& SpatialHashGrid::GetGridablesInCell(int InIndex)
{
    static const std::vector<IGridable*> EmptyCell;

    if (InIndex < 0 || InIndex >= static_cast<int32_t>(Cells.size()))
    {
        return EmptyCell;
    }

    return Cells[InIndex];
}

// À¯´Ö Ãß°¡: O(1)
void SpatialHashGrid::Insert(IGridable* InGridable)
{
    if (!InGridable) return;
    glm::vec3 GridablePosition = InGridable->GetPosition();
    int Index = GetCellIndex(GridablePosition.x, GridablePosition.z);
    if (Index == -1) return;

    Cells[Index].push_back(InGridable);
    InGridable->SetGridIndex(Index);
}

void SpatialHashGrid::Clear()
{
    for (auto& Cell : Cells)
    {
        Cell.clear();
    }
}