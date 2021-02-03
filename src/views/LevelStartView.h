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

#ifndef SUPERPACMAN_LEVELSTARTVIEW_H
#define SUPERPACMAN_LEVELSTARTVIEW_H

#include <IME/graphics/Sprite.h>
#include <IME/core/time/Time.h>
#include "CommonView.h"

namespace pacman {
    /**
     * @brief View displayed before a level is started
     *
     * This view is displayed when a level is played for the first time
     * or when pacman dies but still has lives to continue playing
     */
    class LevelStartView {
    public:
        /**
         * @brief Create level start view
         * @param renderTarget Target view is rendered on
         * @param level Current level
         * @param lives Pacmans current number of lives
         * @param score Current score
         * @param highScore Current high score
         */
        LevelStartView(ime::Window& renderTarget, int level, int lives,
           int score, int highScore);

        /**
         * @brief Initialize the view
         */
        void init();

        /**
         * @brief Update view
         * @param deltaTime Time passed since view was last updated
         */
        void update(ime::Time deltaTime);

        /**
         * @brief Render the view
         * @param renderTarget Target to render view on
         */
        void render(ime::Window& renderTarget);

    private:
        ime::ui::GuiContainer guiContainer_; //!< Container for all widgets in this view
        ime::Vector2u windowSize_;           //!< Size of the games render target
        CommonView commonView_;              //!< View displayed across all states
        ime::Sprite fruit_;                  //!< The the current level fruit
        int level_;                          //!< Current level
        int score_;                          //!< Current player score
        int highScore_;                      //!< Current top score
    };
}

#endif
