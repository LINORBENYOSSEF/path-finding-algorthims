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

struct AlgoAStar {
    Maze* maze = NULL;
    Tile* target = NULL;
    AStarPathNode* foundNode = NULL;

    std::priority_queue<AStarPathNode*, std::vector<AStarPathNode*>, AStarNodeComparator> queue;
    std::vector<Tile*> visited;
};

void astarInit(AlgoAStar& context, Maze& maze, Tile& start, Tile& target);
bool astarIteration(AlgoAStar& context);
void astarFinished(AlgoAStar& context);
