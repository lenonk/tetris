#include <raylib.h>

#include "Game.h"

#include <algorithm>
#include <ctime>
#include <string>

#include "Block.h"
#include "Colors.h"

namespace Tetris {
Game::Game() {
    InitAudioDevice();

    m_music = LoadMusicStream("sounds/music.mp3");
    PlayMusicStream(m_music);

    m_rotateSound = LoadSound("sounds/rotate.mp3");
    m_clearSound = LoadSound("sounds/clear.mp3");
}

Game::Game(const uint16_t screenWidth, const uint16_t screenHeight, const uint8_t targetFPS) : Game() {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_targetFPS = targetFPS;

    SetRandomSeed(time(nullptr));
    ResetBlockArray();

    m_currentBlock = GetRandomBlock();
    m_nextBlock = GetRandomBlock();
}

Game::~Game() {
    UnloadSound(m_rotateSound);
    UnloadSound(m_clearSound);
    UnloadMusicStream(m_music);
    CloseAudioDevice();
}

void
Game::Run() {
    InitWindow(m_screenWidth, m_screenHeight, "Raylib Tetris!");
    SetTargetFPS(m_targetFPS);

    const auto font = LoadFontEx("fonts/monogram.ttf", 64, nullptr, 0);
    while (!WindowShouldClose()) {
        UpdateMusicStream(m_music);
        if (const auto time = GetTime(); !m_paused && !m_gameOver && time - m_lastBlockDropTime > m_blockDropDelay) {
            MoveBlockDown();
            m_lastBlockDropTime = time;
        }
        HandleInput();
        BeginDrawing();
        ClearBackground(Colors::DarkBlue);

        DrawTextEx(font, "Score", {365, 15}, 38, 2, RAYWHITE);
        DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, Colors::LightBlue);

        const auto scoreText = std::string(std::to_string(m_playerScore));
        const auto [x, y] = MeasureTextEx(font, scoreText.c_str(), 38, 2);
        DrawTextEx(font, scoreText.c_str(), {320 + (170 - x) / 2, 65}, 38, 2, RAYWHITE);

        DrawTextEx(font, "Next", {370, 175}, 38, 2, RAYWHITE);
        DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, Colors::LightBlue);

        if (m_gameOver)
            DrawTextEx(font, "GAME OVER", {320, 450}, 38, 2, RAYWHITE);

        Draw();
        EndDrawing();
    }

    CloseWindow();
}

Block
Game::GetRandomBlock() {
    const auto idx = GetRandomValue(0, static_cast<int32_t>(m_blockVector.size()) - 1);
    Block block = m_blockVector[idx];

    m_blockVector.erase(m_blockVector.begin() + idx);

    if (m_blockVector.empty()) {
        ResetBlockArray();
    }

    return block;
}

void
Game::Draw() {
    m_grid.Draw();
    m_currentBlock.Draw();
    switch (m_nextBlock.id()) {
        case BlockType::I:
            m_nextBlock.Draw(255, 290);
            break;
        case BlockType::O:
            m_nextBlock.Draw(255, 280);
            break;
        default:
            m_nextBlock.Draw(270, 270);
            break;
    }
}

void
Game::HandleInput() {
    if (m_gameOver) {
        if (GetKeyPressed() != 0) {
            ResetGame();
        }
        return;
    }

    if (IsKeyPressed(KEY_P)) { m_paused = !m_paused; }

    const auto currentTime = GetTime();
    if (IsKeyDown(KEY_LEFT) && currentTime - m_lastHorizontalMoveTime > m_horizontalMoveDelay) {
        MoveBlockLeft();
        m_lastHorizontalMoveTime = currentTime;
    }
    if (IsKeyDown(KEY_RIGHT) && currentTime - m_lastHorizontalMoveTime > m_horizontalMoveDelay) {
        MoveBlockRight();
        m_lastHorizontalMoveTime = currentTime;
    }
    if (IsKeyDown(KEY_DOWN) && currentTime - m_lastVerticalMoveTime > m_verticalMoveDelay) {
        MoveBlockDown();
        m_playerScore++;
        m_lastVerticalMoveTime = currentTime;
    }
    if (IsKeyDown(KEY_UP) && currentTime - m_lastRotationTime > m_rotationDelay) {
        RotateBlock();
        m_lastRotationTime = currentTime;
    }
}

bool
Game::IsBlockOutsideGrid() {
    return std::ranges::any_of(m_currentBlock.GetCellPositions(), [this](const auto &cell) {
        return m_grid.IsCellOutOfBounds(cell.row, cell.column);
    });
}

bool
Game::BlockFitsOnGrid() {
    return BlockFitsOnGrid(m_currentBlock);
}

bool
Game::BlockFitsOnGrid(Block &block) {
    return std::ranges::all_of(block.GetCellPositions(), [this](const auto &cell) {
        return m_grid.IsCellEmpty(cell.row, cell.column);
    });
}
void
Game::MoveBlockLeft() {
    m_currentBlock.Move(0, -1);

    if (IsBlockOutsideGrid() || !BlockFitsOnGrid()) { MoveBlockRight(); }
}

void
Game::MoveBlockRight() {
    m_currentBlock.Move(0, 1);
    if (IsBlockOutsideGrid()|| !BlockFitsOnGrid()) { MoveBlockLeft(); }
}

void
Game::MoveBlockUp() {
    m_currentBlock.Move(-1, 0);
    if (IsBlockOutsideGrid()) { MoveBlockDown(); }
}

void
Game::MoveBlockDown() {
    m_currentBlock.Move(1, 0);
    if (IsBlockOutsideGrid() || !BlockFitsOnGrid()) {
        MoveBlockUp();
        LockBlock();
        const auto cleared = m_grid.CheckFullRows();
        switch (cleared) {
            case 1: m_playerScore += 100; break;
            case 2: m_playerScore += 300; break;
            case 3: m_playerScore += 500; break;
            case 4: m_playerScore += 1000; break;
            default: break;
        }

        if (cleared) { PlaySound(m_clearSound); }
    }
}

void
Game::ResetGame() {
    m_gameOver = false;
    m_grid.Initialize();
    ResetBlockArray();
    m_currentBlock = GetRandomBlock();
    m_nextBlock = GetRandomBlock();
}

void
Game::RotateBlock() {
    m_currentBlock.Rotate();
    if (IsBlockOutsideGrid() || !BlockFitsOnGrid()) {
        m_currentBlock.UndoRotate();
    }
    else {PlaySound(m_rotateSound);}
}

void
Game::LockBlock() {
    std::ranges::for_each(m_currentBlock.GetCellPositions(), [this](const auto &cell) {
        m_grid.GetGridCells()[cell.row][cell.column] = m_currentBlock.id();
    });

    if (!BlockFitsOnGrid(m_nextBlock)) {
        m_gameOver = true;
        return;
    }

    m_currentBlock = m_nextBlock;
    m_nextBlock = GetRandomBlock();
}

}
