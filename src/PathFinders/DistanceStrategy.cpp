#include "DistanceStrategy.h"
#include <IME/common/Vector2.h>
#include <IME/core/grid/Grid2D.h>
#include <IME/core/object/GridObject.h>
#include <vector>

namespace {
    ///////////////////////////////////////////////////////////////
    ime::Vector2i conIndexToVec(const ime::Index& index) {
        return {index.row, index.colm};
    }

    ///////////////////////////////////////////////////////////////
    bool hasObstacle(const ime::Grid2D& grid, const ime::Index& index) {
        auto& tile = grid.getTile(index);

        if (tile.isCollidable())
            return true;

        bool hasObstacle = false;
        grid.forEachChildInTile(tile, [&hasObstacle](const ime::GridObject* child) {
            if (child->isObstacle() && child->isActive()) {
                hasObstacle = true;

                return;
            }
        });

        return hasObstacle;
    }

    ///////////////////////////////////////////////////////////////
    ime::Index getMinDistanceIndex(std::vector<ime::Index>& neighbours, const ime::Index &targetIndex) {
        ime::Vector2i targetTileVec = conIndexToVec(targetIndex);
        ime::Index minDisIndex = neighbours.at(0);
        float minDistance = conIndexToVec(neighbours.front()).distanceTo(targetTileVec);

        for (int i = 1; i < neighbours.size(); i++) {
            float dist = conIndexToVec(neighbours[i]).distanceTo(targetTileVec);

            if (dist < minDistance) {
                minDistance = dist;
                minDisIndex = neighbours[i];
            }
        }

        return minDisIndex;
    }
} // namespace

///////////////////////////////////////////////////////////////
DistanceStrategy::DistanceStrategy(bool avoidObstacles) : avoidObstacles_(avoidObstacles)
{}

///////////////////////////////////////////////////////////////
void DistanceStrategy::addNeighbours(const ime::Grid2D &grid, const ime::Index& tileIndex) {
    auto static addNeighbour = [this](const ime::Grid2D& grid2D, const ime::Index& index)
    {
        if (grid2D.isIndexValid(index))
        {
            if (avoidObstacles_ && (isVisited(index) || hasObstacle(grid2D, index)))
                return;

            neighbours_.emplace_back(index);
        }
    };

    int row = tileIndex.row;
    int col = tileIndex.colm;

    addNeighbour(grid, ime::Index{row, col - 1}); //Left neighbour
    addNeighbour(grid, ime::Index{row - 1, col}); //Top neighbour
    addNeighbour(grid, ime::Index{row, col + 1}); //Right neighbour
    addNeighbour(grid, ime::Index{row + 1, col}); //Bottom neighbour
}

///////////////////////////////////////////////////////////////
bool DistanceStrategy::isVisited(const ime::Index &tileIndex) const {
    return std::find(visited_.begin(), visited_.end(), tileIndex) != visited_.end();
}

///////////////////////////////////////////////////////////////
std::stack<ime::Index> DistanceStrategy::findPath(const ime::Grid2D &grid, const ime::Index &sourceNode, const ime::Index &targetNode) {
    if (avoidObstacles_ && hasObstacle(grid, targetNode))
        return std::stack<ime::Index>{};

    ime::Index curNode = sourceNode;
    std::stack<ime::Index> path{};
    visited_.push_back(curNode);

    while(curNode != targetNode) {
        addNeighbours(grid, curNode);

        if (neighbours_.empty()) {
            visited_.clear();

            return std::stack<ime::Index>{};
        }
        else {
            curNode = getMinDistanceIndex(neighbours_, targetNode);
            visited_.push_back(curNode);
            neighbours_.clear();
        }

        path.push(curNode);
    }

    // Clear current rub
    visited_.clear();

    // IME v3.2.0 requires that the top of the stack be the first adjacent node
    std::stack<ime::Index> temp{};

    while (!path.empty()) {
        temp.push(path.top());
        path.pop();
    }

    return temp;
}

///////////////////////////////////////////////////////////////
std::string DistanceStrategy::getType() const {
    return "distance";
}
