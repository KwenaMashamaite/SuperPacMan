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

#include "LevelStartScene.h"
#include <IME/core/loop/Engine.h>

namespace pacman {
    LevelStartScene::LevelStartScene(ime::Engine &engine) :
        Scene(engine)
    {}

    void LevelStartScene::onEnter() {
        auto level = cache().getValue<int>("level");
        auto lives = cache().getValue<int>("lives");
        auto score = cache().getValue<int>("score");
        auto highScore = cache().getValue<int>("high-score");
        view_ = std::make_unique<LevelStartView>(engine().getRenderTarget(), level, lives, score, highScore);
        view_->init();

        if (level == 1) {
            sfx_.setSource("beginning.wav");
            sfx_.play();
            timer().setTimeout(ime::seconds(4), [this] {
                engine().popScene();
            });
        } else
            timer().setTimeout(ime::seconds(2), [this] {engine().popScene();});
    }

    void LevelStartScene::render(ime::Window &renderTarget) {
        view_->render(renderTarget);
    }

    void LevelStartScene::update(ime::Time deltaTime) {
        view_->update(deltaTime);
    }

    void LevelStartScene::handleEvent(ime::Event event) {}
}