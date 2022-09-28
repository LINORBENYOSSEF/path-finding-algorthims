#pragma once

#include <cstddef>
#include <cstdint>

enum class TileType {
    SPACE,
    WALL,
    PACKMAN,
    FOOD,
    GHOST
};

struct Tile {
    size_t row;
    size_t col;
    TileType type;

    bool operator==(Tile& other);

    bool canMoveTo();
};

class Maze {
public:
    static constexpr size_t SIZE = 100;

    Maze();

    Tile& tile(size_t row, size_t col);

    void draw();
    Tile* findClosestTileOfType(Tile& tile, TileType type);
    Tile* getRandomTileOfType(TileType type);

private:
    void initTiles();
    void initBoundaries();
    void generateMazeContent();

    Tile tiles[SIZE][SIZE];
};
