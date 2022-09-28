#include <iostream>

#include "characters.h"


Character::Character(Maze& maze1, Tile& startPosition, TileType positionType)
    : maze(maze1)
    , position(nullptr)
    , lastTileType(startPosition.type)
    , positionType(positionType)
{
    moveToTile(startPosition);
}

Tile& Character::currentPosition()
{
    return *position;
}

void Character::moveToTile(Tile& dest)
{
    if (nullptr != position) {
        position->type = lastTileType;
    }

    std::cout << "Move to (" << dest.row << ", " << dest.col << ")" << std::endl;

    position = &dest;
    lastTileType = position->type;
    position->type = positionType;

    if (lastTileType == TileType::FOOD) {
        lastTileType = TileType::SPACE;
        // ate food
    }
}

Packman::Packman(Maze& maze, Tile& position)
    : Character(maze, position, TileType::PACKMAN)
    , pathFindingAlgorithm(maze)
    , destination(nullptr)
{

}

void Packman::update()
{
    if (hasReachedDestination()) {
        Tile* nextDestination = findNextDestination();
        if (nullptr == nextDestination) {
            return;
        }

        std::cout << "New Destination (" << nextDestination->row << ", " << nextDestination->col << ")" << std::endl;

        destination = nextDestination;
    }

    auto& tile = pathFindingAlgorithm.findNextTileToDestination(currentPosition(), *destination);
    moveToTile(tile);
}

Tile* Packman::findNextDestination()
{
    return maze.findClosestTileOfType(currentPosition(), TileType::FOOD);
}

bool Packman::hasReachedDestination()
{
    return nullptr == destination || *destination == currentPosition();
}

Ghost::Ghost(Maze& maze, Packman& packman, Tile& position)
    : Character(maze, position, TileType::GHOST)
    , packman(packman)
    , pathFindingAlgorithm(maze)
{
    pathFindingAlgorithm.allowMovingToTileType(TileType::PACKMAN);
}

void Ghost::update()
{
    auto& tile = pathFindingAlgorithm.findNextTileToDestination(currentPosition(), packman.currentPosition());
    moveToTile(tile);
}
