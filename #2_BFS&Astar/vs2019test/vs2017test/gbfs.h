#pragma once

#include <vector>
#include <queue>

#include "maze.h"
#include "path.h"

// https://www.geeksforgeeks.org/best-first-search-informed-search/


struct GBFSPathNode : public PathNode {
    double cost;
};

struct GBFSNodeComparator
{
public:
    bool operator()(const GBFSPathNode* node1, const GBFSPathNode* node2);
};

struct AlgoGBFS {
    Maze* maze = NULL;
    Tile* target = NULL;
    GBFSPathNode* foundNode = NULL;

    std::priority_queue<GBFSPathNode*, std::vector<GBFSPathNode*>, GBFSNodeComparator> queue;
    std::vector<Tile*> visited;
};

void gbfsInit(AlgoGBFS& context, Maze& maze, Tile& start, Tile& target);
bool gbfsIteration(AlgoGBFS& context);
void gbfsFinished(AlgoGBFS& context);
