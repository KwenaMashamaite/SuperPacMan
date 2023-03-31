#ifndef SUPERPACMAN_DISTANCESTRATEGY_H
#define SUPERPACMAN_DISTANCESTRATEGY_H

#include <IME/core/physics/grid/path/IPathFinderStrategy.h>

/**
 * @brief A path finding strategy that uses the minimum distance between tiles
 *
 * This strategy does not always give the shortest distance. When the grid has
 * no obstacles it mimics breadth first search and when the grid has obstacles
 * that must be avoided it mimics like depth first search, or does it's own thing
 */
class DistanceStrategy : public ime::IPathFinderStrategy {
public:
    /**
     * @brief Constructor
     * @param avoidObstacles True to avoid tiles that have obstacles, false otherwise
     *
     * When @a avoidObstacles is set to true, the path finder strategy will avoid
     * going through tiles that have collidable tiles or grid objects that are
     * set as obstacles. This means a path will only be generated if the target tile
     * is reachable from the starting tile. When it is set to false, a path is always
     * generated, even if it is blocked by an obstacle
     */
    explicit DistanceStrategy(bool avoidObstacles = false);

    /**
     * @brief Generate a path from a source tile to a target tile
     * @param grid The grid to find the path in
     * @param sourceNode The start point
     * @param targetNode The end point
     * @return The generated path
     *
     * The path will be empty if either the source or target indexes are invalid or
     * the target is not reachable and obstacles are avoided.
     *
     * @see DistanceStrategy
     */
    std::stack<ime::Index> findPath(const ime::Grid2D &grid, const ime::Index &sourceNode, const ime::Index &targetNode) override;

    /**
     * @brief Get the type of strategy
     * @return The strategy type
     */
    std::string getType() const override;

private:
    void addNeighbours(const ime::Grid2D &grid, const ime::Index& tileIndex);
    bool isVisited(const ime::Index& tileIndex) const;

private:
    bool avoidObstacles_;
    std::vector<ime::Index> visited_;
    std::vector<ime::Index> neighbours_;
};

#endif
