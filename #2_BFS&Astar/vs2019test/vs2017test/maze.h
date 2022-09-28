#pragma once

#include <cstddef>
#include <cstdint>

enum class TileType {
    SPACE,
    WALL,
    START,
    TARGET,
    BLACK,
    FRINGE,
    PATH
};

struct Tile {
    size_t row;
    size_t col;
    TileType type;
};

struct Maze {
    static constexpr size_t SIZE = 30;
    Tile tiles[SIZE][SIZE];
};

void generateMaze(Maze& maze);
void drawMaze(const Maze& maze);
