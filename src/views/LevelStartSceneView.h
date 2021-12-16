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

#ifndef SUPERPACMAN_LEVELSTARTSCENEVIEW_H
#define SUPERPACMAN_LEVELSTARTSCENEVIEW_H

#include <IME/core/time/Time.h>
#include "Views/CommonView.h"

namespace spm {
    /**
     * @brief View displayed before a level is started
     *
     * This view is displayed before a level is played for the first time
     * after pacman dies but the player has more lives to continue playing the
     * game
     */
    class LevelStartSceneView {
    public:
        /**
         * @brief Constructor
         * @param gui Container for all UI controls in the view
         */
        explicit LevelStartSceneView(ime::ui::GuiContainer& gui);

        /**
         * @brief Initialize the view
         * @param level Current level number
         * @param lives Pacmans current number of lives
         * @param score Current score
         * @param highScore Current high score
         */
        void init(int level, int lives, int score, int highScore);

        /**
         * @brief Update view
         * @param deltaTime Time passed since view was last updated
         */
        void update(ime::Time deltaTime);

    private:
        ime::ui::GuiContainer& gui_; //!< Container for all widgets in this view
        CommonView commonView_;      //!< View displayed across all scenes
    };
}

#endif
