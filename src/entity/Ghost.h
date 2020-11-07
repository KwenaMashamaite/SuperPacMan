/**
 * @brief Enemy class
 */

#ifndef GHOST_H
#define GHOST_H

#include <IME/core/entity/Entity.h>
#include <IME/core/entity/Entity.h>
#include "Movable.h"

namespace SuperPacMan {
    enum class GhostName {
        Blinky, //Red ghost
        Pinky,  //Pink ghost
        Inky,   //Orange ghost
        Clyde   //Blue ghost
    };

    class Ghost : public IME::Entity, public Movable {
    public:
        /**
         * @brief Construct ghost object
         * @param boundingRect Size of the rectangle surrounding the ghost object
         */
        Ghost(GhostName ghostName, const IME::Dimensions &boundingRect);

        /**
         * @brief Get the type of the entity
         * @return The type of the entity
         */
        std::string getType() override;

        /**
         * @brief Get the name of the ghost
         * @return The name of the ghost
         */
        GhostName getGhostName() const;

    private:
        //Some behaviours depend on the type of ghost
        //But this should probably not be in here
        GhostName ghostName_;
    };
}

#endif
