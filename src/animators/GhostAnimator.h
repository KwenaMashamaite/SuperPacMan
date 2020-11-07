/**
 * @brief Defines and controls ghost animations
 */

#ifndef GHOSTANIMATOR_H
#define GHOSTANIMATOR_H

#include <IME/graphics/AnimatableSprite.h>
#include <IME/core/entity/Entity.h>
#include "IEntityAnimator.h"
#include "../entity/Ghost.h"

namespace SuperPacMan {
    class GhostAnimator : public IEntityAnimator {
    public:
        /**
         * @brief Construct ghost animator
         * @param ghostName Name of the ghost to be animated
         * @param sprite The graphical representation of the ghost object
         * @param ghost Target ghost object
         */
        GhostAnimator(GhostName ghostName, IME::Graphics::AnimatableSprite& sprite, IME::Entity &ghost);

        /**
         * @brief Initialize the ghost animator
         *
         * If the animator is not initialized, animations will not play
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

        /**
         * @brief Resets the animation
         */
        void reset();

    private:
        //Name of the ghost
        GhostName ghostName_;
        //Target
        IME::Entity& ghost_;
        //Sprite to animate ghost
        IME::Graphics::AnimatableSprite& ghostSprite_;

        enum class PacManMode {
            Normal,
            Energized,
            SuperSized
        };

        //The current mode pacman is in
        PacManMode pacManMode_;
        //The previous mode pacman was in
        PacManMode prevPacManMode_;
    };
}

#endif
