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

#include "LevelStartState.h"
#include <IME/core/loop/Engine.h>

namespace pacman {
    LevelStartState::LevelStartState(ime::Engine &engine) :
        State(engine),
        isInit_{false}
    {}

    void LevelStartState::onEnter() {
        auto level = engine().getPersistentData().getValueFor<int>("level");
        auto lives = engine().getPersistentData().getValueFor<int>("lives");
        auto score = engine().getPersistentData().getValueFor<int>("score");
        auto highScore = engine().getPersistentData().getValueFor<int>("high-score");
        view_ = std::make_unique<LevelStartView>(engine().getRenderTarget(), level, lives, score, highScore);
        view_->init();

        if (level == 1) {
            sfx_.setSource("beginning.wav");
            sfx_.play();
            engine().setTimeout(ime::seconds(sfx_.getDuration().asSeconds()), [this] {
                engine().popState();
            });
        } else
            engine().setTimeout(ime::seconds(2), [this] {engine().popState();});

        isInit_ = true;
    }

    bool LevelStartState::isEntered() const {
        return isInit_;
    }

    void LevelStartState::render(ime::Window &renderTarget) {
        view_->render(renderTarget);
    }

    void LevelStartState::update(ime::Time deltaTime) {
        view_->update(deltaTime);
    }

    void LevelStartState::fixedUpdate(ime::Time deltaTime) {}

    void LevelStartState::handleEvent(ime::Event event) {}

    void LevelStartState::onPause() {}

    void LevelStartState::onResume() {}

    void LevelStartState::onExit() {}
}