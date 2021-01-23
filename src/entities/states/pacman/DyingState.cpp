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

#include "DyingState.h"
#include <IME/core/event/EventDispatcher.h>
#include <cassert>

namespace pacman {
    DyingState::DyingState(std::shared_ptr<ime::Entity> pacman) {
        assert(std::dynamic_pointer_cast<PacMan>(pacman) && "Cannot create Pacman state for non Pacman object");
        pacman_ = std::move(std::dynamic_pointer_cast<PacMan>(pacman));
    }

    void DyingState::onEntry() {
        pacman_->setNumberOfLives(pacman_->getNumberOfLives() - 1);
        pacman_->getSprite().switchAnimation("dying");
        setTimeout(pacman_->getSprite().getCurrentAnimation()->getDuration(), [this] {
            pacman_->popState();
        });
    }

    void DyingState::update(ime::Time deltaTime) {
        TimedState::update(deltaTime);
    }

    void DyingState::onExit() {
        if (pacman_->isActive())
            ime::EventDispatcher::instance()->dispatchEvent("pacmanRevived", pacman_);
        else
            ime::EventDispatcher::instance()->dispatchEvent("pacmanDied", pacman_);
    }

    void DyingState::onTimeout() {
        callback();
    }
}
