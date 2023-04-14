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

#ifndef SUPERPACMAN_GAMEOVERSCENEVIEW_H
#define SUPERPACMAN_GAMEOVERSCENEVIEW_H

#include <Mighter2d/ui/GuiContainer.h>

namespace spm {
    /**
     * @brief View displayed to user when game transitions to GameOverScene
     */
    class GameOverSceneView {
    public:
        /**
         * @brief Initialize the view
         * @param gui Container for all ui widgets in the view
         * @param wonGame True if player won the game, or false if player lost
         */
        void init(mighter2d::ui::GuiContainer& gui, bool wonGame);

    private:
        /**
         * @brief Create menu displayed before user saves their data
         * @param gui Container for gui widgets
         * @param wonGame True if player won the game or false if player lost
         */
        void createPreSaveMenu(mighter2d::ui::GuiContainer& gui, bool wonGame);

        /**
         * @brief Create menu displayed after user saves their data
         * @param gui Container for gui widgets
         * @param wonGame True if player won the game or false if player lost
         */
        void createNavButtons(mighter2d::ui::GuiContainer& gui, bool wonGame);
    };
}

#endif //SUPERPACMAN_GAMEOVERSCENEVIEW_H
