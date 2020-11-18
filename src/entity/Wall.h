/**
 * @brief Models a wall
 */

#ifndef WALL_H
#define WALL_H

#include <IME/core/entity/Entity.h>

namespace SuperPacMan {
    class Wall : public IME::Entity {
    public:
        /**
         * @brief Construct wall object
         * @param boundingRect Size of the objects bounding rectangle
         *
         * The wall is collidable by default
         */
        explicit Wall(const IME::Vector2u &boundingRect);

        /**
         * @brief Get the type of the object
         * @return The type of the object
         */
        std::string getType() override;
    };
}

#endif
