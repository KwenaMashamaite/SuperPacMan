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

#ifndef SUPERPACMAN_PELLETANIMATIONS_H
#define SUPERPACMAN_PELLETANIMATIONS_H

#include <IME/core/animation/Animation.h>
#include <vector>

namespace spm {
    /**
     * @brief Constructs all the pellet animations
     */
    class PelletAnimations {
    public:
        /**
         * @brief Default constructor
         */
        PelletAnimations();

        /**
         * @brief Create animations for a specific pellet
         * @param tag Pellet to create animations for
         *
         * This function works with the following tags:
         * 1. "powerPellet" = Pellet that makes ghosts vulnerable when eaten
         * 2. "superPellet" = Gives Pacman super powers when eaten
         */
        void createAnimationFor(const std::string& tag);

        /**
         * @brief Get all of the created animations
         * @return A list of all the created animations
         */
        const std::vector<ime::Animation::Ptr>& getAll() const;

    private:
        std::vector<ime::Animation::Ptr> animations_;
        ime::SpriteSheet spritesheet_;
    };
}

#endif
