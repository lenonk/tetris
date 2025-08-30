#pragma once
#include <cstdint>
#include <vector>
#include <map>

#include "Grid.h"
#include "Colors.h"

namespace Tetris {
enum BlockType : uint8_t {
    L = 1,
    J,
    I,
    O,
    S,
    T,
    Z,
};

struct Position {
    int8_t row;
    int8_t column;

    Position(const int8_t row, const int8_t column) : row(row), column(column) {}
};

constexpr uint16_t DefaultBlockXPadding {10};
constexpr uint16_t DefaultBlockYPadding {10};

class Block {
public:
    Block() = default;
    ~Block() = default;

    void Draw(uint16_t offx = DefaultBlockXPadding, uint16_t offy = DefaultBlockYPadding);
    void Move(int8_t rows, int8_t cols);
    void Rotate();
    void UndoRotate();

    [[nodiscard]] std::vector<Position> GetCellPositions();

    [[nodiscard]] uint8_t id() const { return m_id; }
    void id (const uint8_t id) { m_id = id; }

protected:
    int8_t m_rowOffset {0};
    int8_t m_columnOffset {0};
    int8_t m_rotationState {0};
    uint8_t m_id {0};
    uint8_t m_cellSize {DefaultCellSize};
    uint16_t m_xPadding {DefaultBlockXPadding};
    uint16_t m_yPadding {DefaultBlockYPadding};

    std::map<uint8_t, std::vector<Position>> m_cells;
    std::array<Color, Colors::CellColorCount> m_cellColors { Colors::GetCellColors() };
};

class LBlock : public Block {
public:
    LBlock() {
        m_id = BlockType::L;
        m_cells[0] = { {0, 2}, {1, 0}, {1, 1}, {1, 2} };
        m_cells[1] = { {0, 1}, {1, 1}, {2, 1}, {2, 2} };
        m_cells[2] = { {1, 0}, {1, 1}, {1, 2}, {2, 0} };
        m_cells[3] = { {0, 0}, {0, 1}, {1, 1}, {2, 1} };
        Move(0, 3);
    }
};

class JBlock : public Block {
public:
    JBlock() {
        m_id = BlockType::J;
        m_cells[0] = { {0, 0}, {1, 0}, {1, 1}, {1, 2} };
        m_cells[1] = { {0, 1}, {0, 2}, {1, 1}, {2, 1} };
        m_cells[2] = { {1, 0}, {1, 1}, {1, 2}, {2, 2} };
        m_cells[3] = { {0, 1}, {1, 1}, {2, 0}, {2, 1} };
        Move(0, 3);
    }
};

class IBlock : public Block {
public:
    IBlock() {
        m_id = BlockType::I;
        m_cells[0] = { {1, 0}, {1, 1}, {1, 2}, {1, 3} };
        m_cells[1] = { {0, 2}, {1, 2}, {2, 2}, {3, 2} };
        m_cells[2] = { {2, 0}, {2, 1}, {2, 2}, {2, 3} };
        m_cells[3] = { {0, 1}, {1, 1}, {2, 1}, {3, 1} };
        Move(-1, 3);
    }
};

class OBlock : public Block {
public:
    OBlock() {
        m_id = BlockType::O;
        m_cells[0] = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
        m_cells[1] = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
        m_cells[2] = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
        m_cells[3] = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
        Move(0, 4);
    }
};

class SBlock : public Block {
public:
    SBlock() {
        m_id = BlockType::S;
        m_cells[0] = { {0, 1}, {0, 2}, {1, 0}, {1, 1} };
        m_cells[1] = { {0, 1}, {1, 1}, {1, 2}, {2, 2} };
        m_cells[2] = { {1, 1}, {1, 2}, {2, 0}, {2, 1} };
        m_cells[3] = { {0, 0}, {1, 0}, {1, 1}, {2, 1} };
        Move(0, 3);
    }
};

class TBlock : public Block {
public:
    TBlock() {
        m_id = BlockType::T;
        m_cells[0] = { {0, 1}, {1, 0}, {1, 1}, {1, 2} };
        m_cells[1] = { {0, 1}, {1, 1}, {1, 2}, {2, 1} };
        m_cells[2] = { {1, 0}, {1, 1}, {1, 2}, {2, 1} };
        m_cells[3] = { {0, 1}, {1, 0}, {1, 1}, {2, 1} };
        Move(0, 3);
    }
};

class ZBlock : public Block {
public:
    ZBlock() {
        m_id = BlockType::Z;
        m_cells[0] = { {0, 0}, {0, 1}, {1, 1}, {1, 2} };
        m_cells[1] = { {0, 2}, {1, 1}, {1, 2}, {2, 1} };
        m_cells[2] = { {1, 0}, {1, 1}, {2, 1}, {2, 2} };
        m_cells[3] = { {0, 1}, {1, 0}, {1, 1}, {2, 0} };
        Move(0, 3);
    }
};
}
