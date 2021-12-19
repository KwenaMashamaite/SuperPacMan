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
#include <IME/ui/widgets/Label.h>
#include <IME/core/engine/Engine.h>

namespace spm {
    ///////////////////////////////////////////////////////////////
    LevelStartScene::LevelStartScene() :
        view_{gui()}
    {}

    ///////////////////////////////////////////////////////////////
    void LevelStartScene::onEnter() {
        auto level = cache().getValue<int>("CURRENT_LEVEL");
        auto lives = cache().getValue<int>("PLAYER_LIVES");
        auto score = cache().getValue<int>("CURRENT_SCORE");
        auto highScore = cache().getValue<int>("HIGH_SCORE");
        view_.init(level, lives, score, highScore);

        if (level == cache().getValue<int>("BONUS_STAGE"))
            gui().getWidget<ime::ui::Label>("lblLevel")->setText("BONUS STAGE");

        ime::Time sceneDuration = ime::seconds(2);

        static bool playedAudio = false;
        if (level == 1 && !playedAudio) {
            playedAudio = true;
            audio().setMasterVolume(cache().getValue<float>("MASTER_VOLUME"));
            audio().play(ime::audio::Type::Sfx, "beginning.wav");
            sceneDuration = ime::seconds(4.2);
        }

        timer().setTimeout(sceneDuration, [this] {
            engine().popScene();
        });
    }

    ///////////////////////////////////////////////////////////////
    void LevelStartScene::update(ime::Time deltaTime) {
        view_.update(deltaTime);
    }

} // namespace spm