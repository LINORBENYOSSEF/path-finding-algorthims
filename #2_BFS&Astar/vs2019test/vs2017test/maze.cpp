#include "glew.h"
#include <cstdlib>

#include "maze.h"


static void initTiles(Maze& maze)
{
    for (size_t i = 0; i < Maze::SIZE; ++i) {
        for (size_t j = 0; j < Maze::SIZE; ++j) {
            auto& tile = maze.tiles[i][j];

            tile.row = i;
            tile.col = j;
            tile.type = TileType::SPACE;
        }
    }
}

static void initBoundaries(Maze& maze)
{
    for (size_t i = 0; i < Maze::SIZE; ++i) {
        maze.tiles[0][i].type = TileType::WALL;
        maze.tiles[Maze::SIZE - 1][i].type = TileType::WALL;
        maze.tiles[i][0].type = TileType::WALL;
        maze.tiles[i][Maze::SIZE - 1].type = TileType::WALL;
    }
}

static void generateMazeContent(Maze& maze)
{
    for (size_t i = 1; i < Maze::SIZE - 1; ++i) {
        for (size_t j = 1; j < Maze::SIZE - 1; ++j) {
            if (i % 2 == 1) {
                if (rand() % 100 < 10) {
                    maze.tiles[i][j].type = TileType::WALL;
                } else {
                    maze.tiles[i][j].type = TileType::SPACE;
                }
            } else {
                if (rand() % 100 < 40) {
                    maze.tiles[i][j].type = TileType::SPACE;
                } else {
                    maze.tiles[i][j].type = TileType::WALL;
                }
            }
        }
    }
}

void generateMaze(Maze& maze)
{
    initTiles(maze);
    initBoundaries(maze);
    generateMazeContent(maze);
}

static void drawTile(double x, double y, double sizeX, double sizeY)
{
    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x, y + sizeY);
    glVertex2d(x + sizeX, y + sizeY);
    glVertex2d(x + sizeX, y);
    glEnd();
}

void drawMaze(const Maze& maze)
{
    double sizeX = 2.0 / static_cast<double>(Maze::SIZE);
    double sizeY = 2.0 / static_cast<double>(Maze::SIZE);

    for (size_t i = 0; i < Maze::SIZE; ++i) {
        for (size_t j = 0; j < Maze::SIZE; ++j) {
            switch (maze.tiles[i][j].type) {
                case TileType::SPACE:
                    glColor3d(1, 1, 1);
                    break;
                case TileType::WALL:
                    glColor3d(0.2, 0.4, 0.2);
                    break;
                case TileType::START:
                    glColor3d(0, 1, 1);
                    break;
                case TileType::TARGET:
                    glColor3d(1, 0, 0);
                    break;
                case TileType::BLACK:
                    glColor3d(0.8, 1, 0.8);
                    break;
                case TileType::FRINGE:
                    glColor3d(1, 1, 0);
                    break;
                case TileType::PATH:
                    glColor3d(0.8, 0.4, 1);
                    break;
            }

            double x = 2 * (j / static_cast<double>(Maze::SIZE)) - 1;
            double y = 2 * (i / static_cast<double>(Maze::SIZE)) - 1;

            drawTile(x, y, sizeX, sizeY);
        }
    }
}
