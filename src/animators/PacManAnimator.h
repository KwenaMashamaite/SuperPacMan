/**
 * @brief Defines and controls pacman animations
 */

#ifndef PACMANANIMATOR_H
#define PACMANANIMATOR_H

#include <IME/graphics/AnimatableSprite.h>
#include <IME/core/entity/Entity.h>
#include "IEntityAnimator.h"

namespace SuperPacMan {
    class PacManAnimator : public IEntityAnimator {
    public:
        /**
         * @brief Construct a pacman animator object
         * @brief sprite The graphical representation of the pacman object
         * @param pacman Pacman object to animate
         */
        PacManAnimator(IME::Graphics::AnimatableSprite& sprite, IME::Entity &pacman);

        /**
         * @brief Initialize the pacman animator
         *
         * Pacman animations will not play the animator is not initialized
         */
        void initialize() override;

        /**
         * @brief Update the animator
         * @param deltaTime Time passed since the animator was last updated
         *
         * This function determines which frame of the current animation to
         * display. Therefore it must be called at least once per frame
         */
        void update(float deltaTime) override;

    private:
        //Reference to pacman
        IME::Entity& pacman_;
        //Sprite to animate pacman
        IME::Graphics::AnimatableSprite& pacmanSprite_;

        //Pacman size
        enum class Size {
            Normal,
            SuperSize
        };

        //Flags whether pacman is dying or not
        bool isPacmanDying_;
        //Stores the size of pacman
        Size pacmanSize_;
    };
}

#endif
