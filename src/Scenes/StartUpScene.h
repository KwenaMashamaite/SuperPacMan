////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
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

#ifndef SUPERPACMAN_STARTUPSCENE_H
#define SUPERPACMAN_STARTUPSCENE_H

#include <Mighter2d/core/scene/Scene.h>
#include <Mighter2d/ui/GuiContainer.h>
#include <Mighter2d/core/input/Keyboard.h>
#include <Mighter2d/core/time/Timer.h>

namespace spm {
    /**
     * @brief Defines the start up scene of the game
     *
     * This scene is the first to be entered when the game is run. When
     * in this state, the game displays the copyright notice to the player
     * for a predefined amount of time after which the scene removes itself
     * from the game engine and the game transitions to the LoadingScene
     */
    class StartUpScene : public mighter2d::Scene {
    public:
        /**
         * @brief Constructor
         */
        StartUpScene();

        /**
         * @brief Start the scene
         */
        void onStart() override;

        /**
         * @brief Transition to asset loading scene
         */
        void onDestroy() override;

    private:
        /**
         * @brief Start menu pop countdown
         */
        void startCountdown();

        /**
         * @brief Ask player for their name
         */
        void promptName();

        /**
         * @brief Make the scene skippable when enter key is pressed
         *
         * By default, the scene is no skippable
         */
        void enableSceneSkip();

        /**
         * @brief Save the player name and continue to gameplay
         */
        void save();

    private:
        mighter2d::ui::GuiContainer gui_;
        mighter2d::input::Keyboard keyboard_;
        mighter2d::Timer timer_;
    };
}

#endif
