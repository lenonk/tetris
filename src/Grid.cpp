#include "Grid.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <ranges>

namespace Tetris {

Grid::Grid() {
    Initialize();
}

Grid::Grid(const uint8_t rows, const uint8_t columns, const uint8_t cellSize) :
    m_rows(rows), m_columns(columns), m_cellSize(cellSize) {
    Initialize();
}

Grid::~Grid() = default;

void
Grid::Initialize() {
    m_gridCells.resize(m_rows);
    for (auto &row : m_gridCells) {
        row.assign(m_columns, 0);
    }
}

void
Grid::Draw() const {
    for (uint8_t row = 0; row < m_rows; ++row) {
        for (uint8_t col = 0; col < m_columns; ++col) {
            const auto color = m_gridCells[row][col];
            DrawRectangle(col * m_cellSize + 1 + m_xPadding, row * m_cellSize + 1 + m_yPadding, // X and Y position
                          m_cellSize - 1, m_cellSize - 1, // Width and Height
                          m_cellColors[color]); // Color
        }
    }
}

void
Grid::Print() const {
    for (const auto &row : m_gridCells) {
        for (const auto &cell : row) {
            std::print(std::cout, "{} ", cell);
        }
        std::println(std::cout, "");
    }
}

bool
Grid::IsCellEmpty(const int8_t row, const int8_t column) const {
    return m_gridCells[row][column] == 0;
}

bool
Grid::IsCellOutOfBounds(const int8_t row, const int8_t column) const {
    if (row >= 0 && row < m_rows && column >= 0 && column < m_columns) {
        return false;
    }
    return true;
}

void
Grid::ClearRow(const int8_t row) {
    std::ranges::fill(m_gridCells[row], 0);
}

void
Grid::MoveRowDown(const int8_t row, const int8_t numRows) {
    for (const auto cell : std::views::iota(0, m_columns)) {
        m_gridCells[row + numRows][cell] = m_gridCells[row][cell];
        m_gridCells[row][cell] = 0;
    }
}

int
Grid::CheckFullRows() {
    int8_t completed = 0;

    for (const auto row : std::views::iota(0, m_rows) | std::views::reverse) {
        if (const auto row_idx = static_cast<int8_t>(row); IsRowFull(row_idx)) {
            ClearRow(row_idx);
            ++completed;
        }
        else if (completed > 0) {
            MoveRowDown(row_idx, completed);
        }
    }

    return completed;
}

bool
Grid::IsRowFull(const int8_t row) const {
    return std::ranges::all_of(m_gridCells[row], [](const auto &cell) {
        return cell != 0;
    });
}
}
