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

#ifndef SUPERPACMAN_LEVELSTARTSCENE_H
#define SUPERPACMAN_LEVELSTARTSCENE_H

#include <Mighter2d/core/scene/Scene.h>
#include <Mighter2d/ui/GuiContainer.h>
#include <Mighter2d/core/audio/AudioManager.h>
#include "Views/LevelStartSceneView.h"

namespace spm {
    /**
     * @brief Defines a scene displayed before a level is played
     *
     * When in this state the game displays the current level number
     * and fruit along with its associated points for a short period
     * of time before transitioning to the GamePlayScene
     */
    class LevelStartScene : public mighter2d::Scene {
    public:
        /**
         * @brief Constructor
         */
        LevelStartScene();

        /**
         * @brief Start the scene
         */
        void onStart() override;

    private:
        mighter2d::ui::GuiContainer gui_;
        mighter2d::Timer timer_;
        mighter2d::audio::AudioManager audioPlayer_;
        std::unique_ptr<LevelStartSceneView> view_; //!< View for this state
    };
}

#endif
