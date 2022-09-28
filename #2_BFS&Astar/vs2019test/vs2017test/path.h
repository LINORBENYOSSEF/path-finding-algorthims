#pragma once

#include "maze.h"


struct PathNode {
    PathNode* parent;
    Tile* tile;
};

void rewindAndSetPath(PathNode& lastNode);
