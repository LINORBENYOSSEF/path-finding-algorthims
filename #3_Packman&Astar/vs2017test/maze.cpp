#include "glew.h"
#include "glut.h"
#include <cstdlib>

#include "maze.h"


static void drawTile(double x, double y, double sizeX, double sizeY)
{
    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x, y + sizeY);
    glVertex2d(x + sizeX, y + sizeY);
    glVertex2d(x + sizeX, y);
    glEnd();
}

static double distance(Tile& tile1, Tile& tile2)
{
    return fabs(static_cast<int64_t>(tile1.row) - tile2.row) + fabs(static_cast<int64_t>(tile1.col) - tile2.col);
}

bool Tile::operator==(Tile& other)
{
    return row == other.row && col == other.col;
}

bool Tile::canMoveTo()
{
    switch (type) {
        case TileType::SPACE:
        case TileType::FOOD:
            return true;
        default:
            return false;
    }
}

Maze::Maze()
{
    initTiles();
    initBoundaries();
    generateMazeContent();
}

Tile& Maze::tile(size_t row, size_t col)
{
    return tiles[row][col];
}

void Maze::draw()
{
    double sizeX = 2.0 / static_cast<double>(Maze::SIZE);
    double sizeY = 2.0 / static_cast<double>(Maze::SIZE);

    for (size_t i = 0; i < Maze::SIZE; ++i) {
        for (size_t j = 0; j < Maze::SIZE; ++j) {
            switch (tiles[i][j].type) {
                case TileType::SPACE:
                    glColor3d(1, 1, 1);
                    break;
                case TileType::WALL:
                    glColor3d(0.2, 0.4, 0.2);
                    break;
                case TileType::PACKMAN:
                    glColor3d(0, 0, 0);
                    break;
                case TileType::FOOD:
                    glColor3d(0.8, 1, 0.8);
                    break;
                case TileType::GHOST:
                    glColor3d(1, 0.1, 0.1);
                    break;
            }

            double x = 2 * (j / static_cast<double>(Maze::SIZE)) - 1;
            double y = 2 * (i / static_cast<double>(Maze::SIZE)) - 1;

            drawTile(x, y, sizeX, sizeY);
        }
    }
}

Tile* Maze::findClosestTileOfType(Tile& tile, TileType type)
{
    Tile* closest = nullptr;
    int bestDistance = -1;

    for (int i = 0; i < Maze::SIZE; ++i) {
        for (int j = 0; j < Maze::SIZE; ++j) {
            if (tiles[i][j].type == type) {
                if (nullptr == closest) {
                    closest = &tiles[i][j];
                    bestDistance = distance(*closest, tile);
                } else {
                    int cDistance = distance(tiles[i][j], tile);
                    if (cDistance < bestDistance) {
                        closest = &tiles[i][j];
                        bestDistance = cDistance;
                    }
                }
            }
        }
    }

    return closest;
}

Tile* Maze::getRandomTileOfType(TileType type)
{
    size_t row = rand() % Maze::SIZE;
    size_t col = rand() % Maze::SIZE;

    while (tiles[row][col].type != type) {
        row = rand() % Maze::SIZE;
        col = rand() % Maze::SIZE;
    }

    auto& tile = tiles[row][col];
    return &tile;
}

void Maze::initTiles()
{
    for (size_t i = 0; i < Maze::SIZE; ++i) {
        for (size_t j = 0; j < Maze::SIZE; ++j) {
            auto& tile = tiles[i][j];

            tile.row = i;
            tile.col = j;
            tile.type = TileType::SPACE;
        }
    }
}

void Maze::initBoundaries()
{
    for (size_t i = 0; i < Maze::SIZE; ++i) {
        tiles[0][i].type = TileType::WALL;
        tiles[Maze::SIZE - 1][i].type = TileType::WALL;
        tiles[i][0].type = TileType::WALL;
        tiles[i][Maze::SIZE - 1].type = TileType::WALL;
    }
}

void Maze::generateMazeContent()
{
    for (size_t i = 1; i < Maze::SIZE - 1; ++i) {
        for (size_t j = 1; j < Maze::SIZE - 1; ++j) {
            int chance = rand() % 100;
            if (i % 2 == 1) {
                if (chance < 10) {
                    tiles[i][j].type = TileType::WALL;
                } else if (chance < 20) {
                    tiles[i][j].type = TileType::FOOD;
                } else {
                    tiles[i][j].type = TileType::SPACE;
                }
            } else {
                if (rand() % 100 < 40) {
                    tiles[i][j].type = TileType::SPACE;
                } else {
                    tiles[i][j].type = TileType::WALL;
                }
            }
        }
    }
}
