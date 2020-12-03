/**
 * @brief Constructs all the animations for a specific ghost
 */

#ifndef GHOSTANIMATIONS_H
#define GHOSTANIMATIONS_H

#include <IME/core/animation/Animation.h>
#include <vector>
#include <memory>
#include "../entities/Ghost.h"

namespace SuperPacMan {
    class GhostAnimations {
    public:
        /**
         * @brief Create the all animations for the ghost with the specified name
         * @param ghostName Name of the ghost to create animations for
         */
        void createAnimationsFor(Ghost::Name ghostName);

        /**
         * @brief Get all of the created animations
         * @return A list of all the ghost animations
         */
        const std::vector<std::shared_ptr<IME::Animation>>& getAll() const;

    private:
        /**
         * @brief Create normal and flat movement animations
         * @param ghostName Name of the ghost to create the animations for
         */
        void createMovementAnimations(Ghost::Name ghostName);

        /**
         * @brief Create animations for when a ghost is eaten
         */
        void createEatenAnimations();

        /**
         * @brief Create animations for when the ghost is frightened
         */
        void createFrightenedAnimations();

        /**
         * @brief Create a movement animation
         * @param name Name of the animation
         * @param startPos Starting position of the frame on the spritesheet
         * @param arrangement Arrangement of the animation frames on the spritesheet
         */
        void createAnimation(const std::string& name, IME::Vector2i startPos,
            IME::Arrangement arrangement = IME::Arrangement::Horizontal);

    private:
        //Stores all the created animations
        std::vector<std::shared_ptr<IME::Animation>> animations_;
    };
}

#endif
