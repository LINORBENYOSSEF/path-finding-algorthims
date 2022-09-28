#pragma once

#include <vector>

#include "maze.h"


struct PathNode {
    PathNode* parent;
    Tile* tile;
};

struct Path {
    std::vector<Tile*> path;
};

Path rewindAndSetPath(PathNode& lastNode);


class PathFindingAlgorithm {
public:
    virtual Path findPath(Tile& start, Tile& end) = 0;
    virtual Tile& findNextTileToDestination(Tile& start, Tile& end) = 0;
};
