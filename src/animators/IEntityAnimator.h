/**
 * @brief Abstract base class for a game object animator
 */

#ifndef IENTITYANIMATOR_H
#define IENTITYANIMATOR_H

namespace SuperPacMan {
    class IEntityAnimator {
    public:
        /**
         * @brief Initialize the entity animator
         *
         * This function must be called after construction in order for the
         * the animator to determine which animation to play and how long it
         * should play
         */
        virtual void initialize() = 0;

        /**
         * @brief Update the animator
         * @param deltaTime Time passed since the animator was last updated
         *
         * This function determines which frame of the current animation to
         * display. Therefore it must be called at least once per frame
         */
        virtual void update(float deltaTime) = 0;
    };
}

#endif
