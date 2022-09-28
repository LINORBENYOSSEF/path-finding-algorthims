#include <stack>

#include "path.h"


Path rewindAndSetPath(PathNode& lastNode)
{
    Path path;
    std::stack<Tile*> stack;

    PathNode* current = &lastNode;
    while (nullptr != current) {
        stack.push(current->tile);
        current = current->parent;
    }

    while (!stack.empty()) {
        path.path.push_back(stack.top());
        stack.pop();
    }

    return path;
}
