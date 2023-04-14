////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#include <Mighter2d/core/animation/Animation.h>
#include <vector>

namespace spm {
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
         * @param tag The tag of the ghost
         *
         * This function recognises the following tags:
         * 1. "blinky" = Red ghost
         * 2. "pinky"  = Pink ghost
         * 3. "inky"   = Orange ghost
         * 4. "clyde"  = Cyan ghost
         */
        void createAnimationsFor(const std::string& tag);

        /**
         * @brief Get all of the created animations
         * @return A list of all the ghost animations
         */
        const std::vector<mighter2d::Animation::Ptr>& getAll() const;

    private:
        /**
         * @brief Create a movement animation
         * @param name Name of the animation
         * @param startPos Starting position of the frame on the spritesheet
         */
        void createAnimation(const std::string& name, mighter2d::Index startPos);

        /**
         * @brief Create normal and flat movement animations
         * @param tag The tag of the ghost to create the animation for
         */
        void createMovementAnimations(const std::string& tag);

        /**
         * @brief Create animations for when the ghost is frightened
         */
        void createFrightenedAnimations();

        /**
         * @brief Create animations for when a ghost is eaten
         */
        void createEatenAnimations();

    private:
        std::vector<mighter2d::Animation::Ptr> animations_;
        mighter2d::SpriteSheet spritesheet_;
    };
}

#endif
