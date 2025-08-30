#pragma once

#include <raylib.h>
#include <array>
#include <cstdint>

namespace Tetris::Colors {
constexpr uint8_t CellColorCount {8};

// Cell colors
extern const Color DarkGray;
extern const Color Green;
extern const Color Red;
extern const Color Orange;
extern const Color Yellow;
extern const Color Purple;
extern const Color Cyan;
extern const Color Blue;

// UI colors
extern const Color LightBlue;
extern const Color DarkBlue;

std::array<Color, CellColorCount> GetCellColors();
}