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

#ifndef SUPERPACMAN_GAMEOVERSCENE_H
#define SUPERPACMAN_GAMEOVERSCENE_H

#include <Mighter2d/core/scene/Scene.h>
#include <Mighter2d/ui/GuiContainer.h>

namespace spm {
    /**
     * @brief Game over scene of the game
     */
    class GameOverScene : public mighter2d::Scene {
    public:
        /**
         * @brief Constructor
         */
        GameOverScene();

        /**
         * @brief Start the scene
         */
        void onStart() override;

    private:
        /**
         * @brief Update in game and on disk leaderboard
         */
        void updateLeaderboard();

        /**
         * @brief Initialize the scene gui
         */
        void initGui();

        /**
         * @brief Initialize scene view button events
         */
        void initButtonEvents();

    private:
        mighter2d::ui::GuiContainer gui_;
    };
}

#endif //SUPERPACMAN_GAMEOVERSCENE_H
