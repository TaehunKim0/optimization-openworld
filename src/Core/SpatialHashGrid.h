#pragma once
#include <vector>
#include <string>

class IGridable;
class SpatialHashGrid 
{
private:
    float CellSize;
    int Rows, Cols;
    // 메모리 연속성을 극대화하기 위해 1차원 벡터로 관리 (Grid[y * cols + x])

    std::vector<std::vector<IGridable*>> Cells;

public:
    SpatialHashGrid(float InWidth, float InHeight, float InCellSize);

    int GetCellIndex(float InX, float InZ);
    int GetCols() const { return Cols; }
    int GetRows() const { return Rows; }
    float GetCellSize() const { return CellSize; }

    const std::vector<IGridable*>& SpatialHashGrid::GetGridablesInCell(int InIndex);

    void Insert(IGridable* InGridable);
    void Clear();
};