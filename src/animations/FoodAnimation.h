/**
 * @brief Constructs a food animation
 *
 * This animation is a single looped animation of all the fruits pacman
 * can eat. It is played when a star appears in the game
 */

#ifndef FOODANIMATION_H
#define FOODANIMATION_H

#include <IME/core/animation/Animation.h>
#include <memory>

namespace SuperPacMan {
    class FoodAnimation {
    public:
        /**
         * @brief Constructor
         */
        FoodAnimation();

        /**
         * @brief Get the food animation
         * @return The food animation
         */
        std::shared_ptr<IME::Animation> getAnimation();

    private:
        //Food animation ptr
        std::shared_ptr<IME::Animation> animation_;
    };
}

#endif
