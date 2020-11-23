/**
 * @brief Defines a bunch of helper functions
 */

#ifndef Utils_H
#define Utils_H

#include <IME/core/tilemap/TileMap.h>
#include <unordered_map>
#include <vector>
#include <memory>

using ObjectsContainer = std::unordered_map<std::string, std::vector<std::shared_ptr<IME::Entity>>>;

namespace SuperPacMan::Utils {
    /**
     * @brief Create objects based on the grid Id
     * @param grid Grid to parse
     * @return Created objects
     *
     * This functions looks for specific id's in the grid and creates an object
     * at that grid position corresponding to the id
     */
    extern ObjectsContainer createObjects(IME::TileMap& grid);

    /**
     * @brief Remove all dead entities from the entity container
     */
    extern void removeDeadObjects(std::vector<std::shared_ptr<IME::Entity>>& entities);
}

#endif
