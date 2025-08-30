#pragma once

#include "Block.h"
#include "Grid.h"

namespace Tetris {

constexpr uint16_t DefaultScreenWidth {500};
constexpr uint16_t DefaultScreenHeight {620};

class Game final {
public:
    Game();
    Game(uint16_t screenWidth, uint16_t screenHeight, uint8_t targetFPS = 60);
    ~Game();

    void Run();

private:
    bool m_paused           {false};
    bool m_gameOver         {false};
    uint8_t  m_targetFPS    {60};
    uint16_t m_screenWidth  {DefaultScreenWidth};
    uint16_t m_screenHeight {DefaultScreenHeight};
    uint32_t m_playerScore  {0};

    double m_lastHorizontalMoveTime {0.0f};
    double m_lastVerticalMoveTime   {0.0f};
    double m_lastRotationTime       {0.0f};
    double m_lastBlockDropTime      {0.0f};

    const float m_blockDropDelay        {0.3f};
    const float m_horizontalMoveDelay   {0.1f};
    const float m_verticalMoveDelay     {0.05f};
    const float m_rotationDelay         {0.2f};

    Grid m_grid;
    Block m_currentBlock;
    Block m_nextBlock;
    Music m_music {};
    Sound m_rotateSound {};
    Sound m_clearSound {};

    std::vector<Block> m_blockVector {};

    void Draw();
    void HandleInput();
    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockUp();

    void LockBlock();

    void MoveBlockDown();
    void ResetGame();
    void RotateBlock();

    bool IsBlockOutsideGrid();
    bool BlockFitsOnGrid();
    bool BlockFitsOnGrid(Block &block);
    Block GetRandomBlock();

    void ResetBlockArray() {
        m_blockVector =  { LBlock{}, JBlock{}, IBlock{}, OBlock{}, SBlock{}, TBlock{}, ZBlock{} };
    }
};
}
