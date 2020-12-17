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

#ifndef PACMANANIMATIONS_H
#define PACMANANIMATIONS_H

#include <IME/core/animation/Animation.h>
#include <IME/common/Rect.h>
#include <vector>
#include <memory>

namespace SuperPacMan {
    /**
     * @brief Constructs all the pacman animations
     */
    class PacManAnimations {
    public:
        /**
         * @brief Create the pacman animations
         */
        void create();

        /**
         * @brief Get all of the created animations
         * @return A list of all the pacman animations
         */
        const std::vector<std::shared_ptr<IME::Animation>>& getAll() const;

    private:
        /**
         * @brief Create an animation
         * @param name Name of the animation
         * @param rect Animation frame
         */
        void createAnimation(const std::string& name, IME::IntRect rect);

    private:
        std::vector<std::shared_ptr<IME::Animation>> animations_;
    };
}

#endif
