#pragma once

#include "maze.h"
#include "astar.h"


class Character {
public:
    Character(Maze& maze, Tile& position, TileType positionType);

    virtual void update() = 0;

    Tile& currentPosition();

protected:
    void moveToTile(Tile& dest);

    Maze& maze;

private:
    Tile* position;
    TileType lastTileType;
    TileType positionType;
};

class Packman : public Character {
public:
    Packman(Maze& maze, Tile& position);

    void update() override;

private:
    Tile* findNextDestination();
    bool hasReachedDestination();

    AStarPathFindingAlgorithm pathFindingAlgorithm;
    Tile* destination;
};

class Ghost : public Character {
public:
    Ghost(Maze& maze, Packman& packman, Tile& position);

    void update() override;

private:
    Packman& packman;

    AStarPathFindingAlgorithm pathFindingAlgorithm;
};
