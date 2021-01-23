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

#ifndef SUPERPACMAN_INTROVIEW_H
#define SUPERPACMAN_INTROVIEW_H

#include "CommonView.h"
#include <IME/core/time/Time.h>

namespace pacman {
    /**
     * @brief Frontend for the intro state
     */
    class IntroView {
    public:
        /**
         * @brief Construct the view
         * @param renderTarget Target to render view on
         */
        explicit IntroView(ime::Window& renderTarget);

        /**
         * @brief Initialize the view
         * @param highScore Highest score from last gameplay
         */
        void init(int highScore);
        /**
         * @brief Render the view
         * @param renderTarget Target to render view on
         */
        void render(ime::Window& renderTarget);

        /**
         * @brief Update view
         * @param deltaTime Time passed since view was last updated
         */
        void update(ime::Time deltaTime);

    private:
        CommonView commonView_;              //!< View displayed across all states
        ime::ui::GuiContainer guiContainer_; //!< Container for all widgets in view
        ime::Vector2u windowSize_;           //!< Size of the games render target
    };
}

#endif
