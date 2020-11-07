/**
 * @brief Defines and controls pellet animations
 */

#ifndef PELLETANIMATOR_H
#define PELLETANIMATOR_H

#include <IME/graphics/AnimatableSprite.h>
#include <IME/core/entity/Entity.h>
#include "IEntityAnimator.h"
#include "../entity/Pellet.h"

namespace SuperPacMan {
    class PelletAnimator : public IEntityAnimator {
    public:
        /**
         * @brief Construct a pellet animator
         * @param pelletType The type of the pellet to be animated
         * @param sprite The graphical representation of the pellet object
         * @param pellet Target Pellet object to animate
         */
        PelletAnimator(PelletType pelletType,
            IME::Graphics::AnimatableSprite& sprite,
            IME::Entity &pellet);

        /**
         * @brief Initialize the pellet animator
         *
         * Animations will not play if this function is not called
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
        //The type of pellet to be animated
        PelletType pelletType_;
        //Graphical representation of the pellet object
        IME::Graphics::AnimatableSprite& pelletSprite_;
        //The pellet object to be animated
        IME::Entity& pellet_;
    };
}

#endif
