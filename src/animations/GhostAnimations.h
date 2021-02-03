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

#ifndef SUPERPACMAN_GHOSTANIMATIONS_H
#define SUPERPACMAN_GHOSTANIMATIONS_H

#include <IME/core/animation/Animation.h>
#include <vector>
#include <memory>
#include "../entities/Ghost.h"

namespace pacman {
    /**
     * @brief Constructs all the animations for a specific ghost
     */
    class GhostAnimations {
    public:
        /**
         * @brief Default constructor
         */
        GhostAnimations();

        /**
         * @brief Create animations for a ghost with the specified name
         * @param ghostName Name of the ghost to create animations for
         */
        void createAnimationsFor(Ghost::Name ghostName);

        /**
         * @brief Get all of the created animations
         * @return A list of all the ghost animations
         */
        const std::vector<ime::Animation::sharedPtr>& getAll() const;

    private:
        /**
         * @brief Create a movement animation
         * @param name Name of the animation
         * @param startPos Starting position of the frame on the spritesheet
         */
        void createAnimation(const std::string& name, ime::Index startPos);

        /**
         * @brief Create normal and flat movement animations
         * @param ghostName Name of the ghost to create the animations for
         */
        void createMovementAnimations(Ghost::Name ghostName);

        /**
         * @brief Create animations for when the ghost is frightened
         */
        void createFrightenedAnimations();

        /**
         * @brief Create animations for when a ghost is eaten
         */
        void createEatenAnimations();

    private:
        std::vector<ime::Animation::sharedPtr> animations_;
        ime::SpriteSheet spritesheet_;                      //!< Spr
    };
}

#endif
