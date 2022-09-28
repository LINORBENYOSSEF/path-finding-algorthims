#pragma once

#include <vector>
#include <queue>

#include "maze.h"
#include "path.h"

// https://www.geeksforgeeks.org/a-search-algorithm/

struct AStarPathNode : public PathNode {
    double hcost;
    double gcost;
};

struct AStarNodeComparator
{
public:
    bool operator()(const AStarPathNode* node1, const AStarPathNode* node2);
};

class AStarPathFindingContext {
public:
    AStarPathFindingContext(Maze& maze, Tile& start, Tile& end, std::vector<TileType>& extraAllowedTypes);
    ~AStarPathFindingContext();

    bool iteratePath();
    Path finishedPathFinding();

private:
    double calculateHCost(const Tile& tile);
    bool wasTileVisited(const Tile& tile);

    Maze& maze;
    Tile& target;
    std::vector<TileType> extraAllowedTypes;

    AStarPathNode* foundNode;
    std::priority_queue<AStarPathNode*, std::vector<AStarPathNode*>, AStarNodeComparator> queue;
    std::vector<const Tile*> visited;
};


class AStarPathFindingAlgorithm : public PathFindingAlgorithm {
public:
    AStarPathFindingAlgorithm(Maze& maze);

    Path findPath(Tile& start, Tile& end) override;
    Tile& findNextTileToDestination(Tile& start, Tile& end) override;

    void allowMovingToTileType(TileType type);

private:
    Maze& maze;
    std::vector<TileType> extraAllowedTypes;
};