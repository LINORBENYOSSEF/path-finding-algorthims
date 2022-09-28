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

static double calculateHCost(const Tile& tile, const Tile& target)
{
    return fabs(static_cast<int64_t>(tile.row)- static_cast<int64_t>(target.row)) + fabs(static_cast<size_t>(tile.col) - static_cast<size_t>(target.col));
}

static bool wasTileVisited(const AlgoAStar& context, const Tile& tile)
{
    return std::find(context.visited.begin(),
                     context.visited.end(),
                     &tile)
           != context.visited.end();
}

void astarInit(AlgoAStar& context, Maze& maze, Tile& start, Tile& target)
{
    context.maze = &maze;
    context.target = &target;
    context.foundNode = nullptr;

    auto startNode = new AStarPathNode;
    startNode->tile = &start;
    startNode->parent = nullptr;
    startNode->hcost = calculateHCost(start, target);
    startNode->gcost = 0;
    context.queue.push(startNode);
    context.visited.push_back(&start);
}

bool astarIteration(AlgoAStar& context)
{
    if(context.queue.empty()) {
        return true;
    }

    auto nextNode = context.queue.top();
    context.queue.pop();

    if (TileType::TARGET == nextNode->tile->type) {
        context.foundNode = nextNode;
        return true;
    }

    auto row = nextNode->tile->row;
    auto col = nextNode->tile->col;

    std::cout << "Running from ("
              << row << ", " << col << ")"
              << std::endl;

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

        auto& tile = context.maze->tiles[point.first][point.second];
        if (TileType::SPACE != tile.type && TileType::TARGET != tile.type) {
            continue;
        }

        if (!wasTileVisited(context, tile)) {
            std::cout << "\tAt node ("
                      << point.first << ", " << point.second << ")"
                      << std::endl;

            context.visited.push_back(&tile);

            auto node = new AStarPathNode;
            node->tile = &tile;
            node->parent = nextNode;
            node->hcost = calculateHCost(tile, *context.target);
            node->gcost = nextNode->gcost + 1;
            context.queue.push(node);

            if (tile.type != TileType::TARGET) {
                tile.type = TileType::BLACK;
            }
        }
    }

    return false;
}

void astarFinished(AlgoAStar& context)
{
    if (nullptr != context.foundNode) {
        rewindAndSetPath(*context.foundNode);
    }

    PathNode* current = context.foundNode;
    while (nullptr != current) {
        auto* node = current;
        current = current->parent;
        delete node;
    }

    while (!context.queue.empty()) {
        auto* node = context.queue.top();
        context.queue.pop();
        delete node;
    }

    context.visited.clear();
}
