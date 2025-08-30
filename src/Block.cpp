#include "Block.h"

namespace Tetris {
void
Block::Draw(uint16_t offx, uint16_t offy) {
    for (const auto &[row, column] : GetCellPositions()) {
        DrawRectangle(
            column * m_cellSize + 1 + offx, row * m_cellSize + 1 + offy, // X and Y position
            m_cellSize - 1, m_cellSize -1, // Width and Height
            m_cellColors[m_id]); //color
    }
}

void
Block::Move(const int8_t rows, const int8_t cols) {
    m_rowOffset += rows;
    m_columnOffset += cols;
}

void
Block::Rotate() {
    m_rotationState = (m_rotationState + 1) % 4;
}

void
Block::UndoRotate() {
    m_rotationState = (m_rotationState - 1 + 4) % 4;
}

std::vector<Position>
Block::GetCellPositions() {
    std::vector<Position> new_positions;

    for (const auto &[row, column] : m_cells[m_rotationState]) {
        new_positions.emplace_back(row + m_rowOffset, column + m_columnOffset);
    }

    return new_positions;
}
}