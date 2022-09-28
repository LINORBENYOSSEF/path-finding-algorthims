#include <cmath>
#include <algorithm>
#include <iostream>

#include "astar.h"


bool AStarNodeComparator::operator()(const AStarPathNode* node1, const AStarPathNode* node2)
{
    // https://en.cppreference.com/w/cpp/container/priority_queue
    // Note that the Compare parameter is defined such that it returns true if its first argument comes before its second argument in a weak ordering.
    // That is, the front of the queue contains the "last" element according to the weak ordering imposed by Compare.
    return node1->hcost + node1->gcost > node2->hcost + node2->gcost;
}

AStarPathFindingContext::AStarPathFindingContext(Maze& maze, Tile& start, Tile& end, std::vector<TileType>& extraAllowedTypes)
    : maze(maze)
    , target(end)
    , foundNode(nullptr)
    , extraAllowedTypes(extraAllowedTypes)
{
    auto startNode = new AStarPathNode;
    startNode->tile = &start;
    startNode->parent = nullptr;
    startNode->hcost = calculateHCost(start);
    startNode->gcost = 0;
    queue.push(startNode);
    visited.push_back(&start);
}

AStarPathFindingContext::~AStarPathFindingContext()
{
    PathNode* current = foundNode;
    while (nullptr != current) {
        auto* node = current;
        current = current->parent;
        delete node;
    }

    while (!queue.empty()) {
        auto* node = queue.top();
        queue.pop();
        delete node;
    }
}

bool AStarPathFindingContext::iteratePath()
{
    if(queue.empty()) {
        return true;
    }

    auto nextNode = queue.top();
    queue.pop();

    if (target.col == nextNode->tile->col &&
        target.row == nextNode->tile->row) {
        foundNode = nextNode;
        return true;
    }

    auto row = nextNode->tile->row;
    auto col = nextNode->tile->col;

    std::pair<size_t, size_t> points[] = {
            {row, col - 1},
            {row, col + 1},
            {row - 1, col},
            {row + 1, col},
    };

    for (auto& point : points) {
        if (point.first >= Maze::SIZE || point.second >= Maze::SIZE) {
            continue;
        }

        auto& tile = maze.tile(point.first, point.second);
        if (!tile.canMoveTo() &&
            std::find(extraAllowedTypes.begin(), extraAllowedTypes.end(), tile.type)
            == extraAllowedTypes.end()) {
            continue;
        }

        if (!wasTileVisited(tile)) {
            visited.push_back(&tile);

            auto node = new AStarPathNode;
            node->tile = &tile;
            node->parent = nextNode;
            node->hcost = calculateHCost(tile);
            node->gcost = nextNode->gcost + 1;
            queue.push(node);
        }
    }

    return false;
}

Path AStarPathFindingContext::finishedPathFinding()
{
    if (nullptr != foundNode) {
        return rewindAndSetPath(*foundNode);
    }

    return {};
}

double AStarPathFindingContext::calculateHCost(const Tile& tile)
{
    return fabs(static_cast<int64_t>(tile.row) - static_cast<int64_t>(target.row)) +
        fabs(static_cast<int64_t>(tile.col) - static_cast<int64_t>(target.col));
}

bool AStarPathFindingContext::wasTileVisited(const Tile& tile)
{
    return std::find(visited.begin(),
                     visited.end(),
                     &tile)
           != visited.end();
}

AStarPathFindingAlgorithm::AStarPathFindingAlgorithm(Maze& maze)
    : maze(maze)
    , extraAllowedTypes()
{

}

Path AStarPathFindingAlgorithm::findPath(Tile& start, Tile& end)
{
    AStarPathFindingContext context(maze, start, end, extraAllowedTypes);
    while (!context.iteratePath());

    return context.finishedPathFinding();
}

Tile& AStarPathFindingAlgorithm::findNextTileToDestination(Tile& start, Tile& end)
{
    auto path = findPath(start, end);
    if (path.path.empty()) {
        return start;
    }

    // first element is the starting point
    return *path.path.at(1);
}

void AStarPathFindingAlgorithm::allowMovingToTileType(TileType type)
{
    extraAllowedTypes.push_back(type);
}
