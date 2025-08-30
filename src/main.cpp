#include "Game.h"

int32_t
main() {
    Tetris::Game tetris(Tetris::DefaultScreenWidth, Tetris::DefaultScreenHeight);
    tetris.Run();
}
