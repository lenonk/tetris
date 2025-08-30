#pragma once
#include <vector>
#include <cstdint>

#include "Colors.h"

namespace Tetris {

constexpr uint8_t DefaultRowCount {20};
constexpr uint8_t DefaultColumnCount {10};
constexpr uint8_t DefaultCellSize {30};

class Grid final {
public:
    Grid();
    Grid(uint8_t rows, uint8_t columns, uint8_t cellSize = DefaultCellSize);
    ~Grid();

    void Initialize();
    void Draw() const;
    void Print() const;

    [[nodiscard]] int CheckFullRows();
    [[nodiscard]] bool IsCellEmpty(int8_t row, int8_t column) const;
    [[nodiscard]] bool IsCellOutOfBounds(int8_t row, int8_t column) const;
    [[nodiscard]] std::vector<std::vector<uint8_t>> &GetGridCells() { return m_gridCells; }

private:
    int8_t m_rows      {DefaultRowCount};
    int8_t m_columns   {DefaultColumnCount};
    int8_t m_cellSize  {DefaultCellSize};

    uint8_t m_xPadding {10};
    uint8_t m_yPadding {10};

    std::vector<std::vector<uint8_t>> m_gridCells {};
    std::array<Color, Colors::CellColorCount> m_cellColors { Colors::GetCellColors()};

    void ClearRow(int8_t row);
    void MoveRowDown(int8_t row, int8_t numRows);
    [[nodiscard]] bool IsRowFull(int8_t row) const;
};
}

