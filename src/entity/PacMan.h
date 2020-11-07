/**
 * @brief Models a player object
 */

#ifndef PACMAN_H
#define PACMAN_H

#include <IME/core/entity/Entity.h>
#include "Movable.h"

namespace SuperPacMan {
    class PacMan : public IME::Entity, public Movable {
    public:
        /**
         * @brief Construct pacman object
         * @param boundingRect Bounding rectangle of pacman object
         */
        explicit PacMan(const IME::Dimensions &boundingRect);

        /**
         * @brief Set the number of lives pacman has
         * @param numOfLives New number of lives
         *
         * The number of lives cannot be less than zero. If the number of lives
         * is zero and pacman is not currently invincible, he will instantly die
         * otherwise if his invincible, nothing will happen. Pacman has one life
         * by default
         */
        void setNumberOfLives(unsigned int numOfLives);

        /**
         * @brief Get the number of lives pacman currently has
         * @return The number of lives pacman currently has
         */
        unsigned int getNumberOfLives() const;

        /**
         * @brief Get the type of the entity
         * @return The type of the entity
         */
        std::string getType() override;

    private:
        //Number of lives
        unsigned int numberOfLives_;
    };
}

#endif
