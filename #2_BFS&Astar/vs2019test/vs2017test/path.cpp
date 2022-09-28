#include "path.h"


void rewindAndSetPath(PathNode& lastNode)
{
    PathNode* current = &lastNode;
    while (nullptr != current) {
        current->tile->type = TileType::PATH;
        current = current->parent;
    }
}
