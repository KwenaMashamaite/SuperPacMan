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

#include "LevelStartScene.h"
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/core/engine/Engine.h>

namespace spm {
    ///////////////////////////////////////////////////////////////
    LevelStartScene::LevelStartScene() :
        view_{nullptr}
    {}

    ///////////////////////////////////////////////////////////////
    void LevelStartScene::onEnter() {
        view_ = new LevelStartSceneView(getGui());
        auto level = getCache().getValue<int>("CURRENT_LEVEL");
        auto lives = getCache().getValue<int>("PLAYER_LIVES");
        auto score = getCache().getValue<int>("CURRENT_SCORE");
        auto highScore = getCache().getValue<int>("HIGH_SCORE");
        view_->init(level, lives, score, highScore);

        if (level == getCache().getValue<int>("BONUS_STAGE"))
            getGui().getWidget<mighter2d::ui::Label>("lblLevel")->setText("BONUS STAGE");

        mighter2d::Time sceneDuration = mighter2d::seconds(2);

        static bool playedAudio = false;
        if (level == 1 && !playedAudio) {
            playedAudio = true;
            getAudio().setMasterVolume(getCache().getValue<float>("MASTER_VOLUME"));
            getAudio().play(mighter2d::audio::Type::Sfx, "beginning.wav");
            sceneDuration = mighter2d::seconds(4.2);
        }

        getTimer().setTimeout(sceneDuration, [this] {
            getEngine().popScene();
        });
    }

    ///////////////////////////////////////////////////////////////
    LevelStartScene::~LevelStartScene() {
        delete view_;
    }

} // namespace spm