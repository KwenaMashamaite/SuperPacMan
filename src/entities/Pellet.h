////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#ifndef SUPERPACMAN_PELLET_H
#define SUPERPACMAN_PELLET_H

#include <IME/core/entity/Entity.h>
#include <IME/graphics/AnimatableSprite.h>
#include "IEatable.h"

namespace pacman {
    /**
     * @brief Type of pellets in the game
     */
    enum class PelletType {
        PowerPellet, //!< Makes ghosts vulnerable when eaten
        SuperPellet  //!< Gives Pacman super powers when eaten
    };

    /**
     * @brief A pellet that can be eaten by Pacman
     */
    class Pellet : public ime::Entity, public IEatable {
    public:
        /**
         * @brief Construct a pellet
         * @brief pelletType Type of the pellet
         * @param boundingRect Size of the pellets bounding rectangle
         */
        explicit Pellet(PelletType pelletType, const ime::Vector2u &boundingRect);

        /**
         * @brief Eat pellet
         */
        void eat() override;

        /**
         * @brief Check if pellet is eaten or not
         * @return True if eaten or false if not eaten
         */
        bool isEaten() const override;

        /**
         * @brief Get the type of this pellet
         * @return The type of this pellet
         */
        PelletType getPelletType() const;

        /**
         * @brief Get the class type
         * @return Name of the concrete class the pellet belongs to
         */
        std::string getClassType() override;

        /**
         * @brief Get the pellets graphical representation
         * @return The pellets graphical representation
         */
        ime::AnimatableSprite& getSprite();

        /**
         * @brief Update pellet
         * @param deltaTime Time passed since pellet was last updated
         */
        void update(float deltaTime);

    private:
        PelletType pelletType_;                  //!< The type of this pellet
        ime::AnimatableSprite sprite_; //!< Pellets graphical representation
    };
}

#endif
