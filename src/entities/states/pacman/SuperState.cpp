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

#include "SuperState.h"
#include "../../../common/Constants.h"
#include "../../../utils/Utils.h"
#include <cassert>

namespace pacman {
    SuperState::SuperState(std::shared_ptr<ime::Entity> pacman) : isPacmanFlashing_{false} {
        assert(std::dynamic_pointer_cast<PacMan>(pacman) && "Cannot create Pacman state for non Pacman object");
        pacman_ = std::move(std::dynamic_pointer_cast<PacMan>(pacman));
    }

    void SuperState::onEntry() {
        pacman_->setSpeed(Constants::PacManSuperSpeed);
        pacman_->setVulnerable(false);
        triggerAnimationSwitch();
    }

    void SuperState::update(ime::Time deltaTime) {
        TimedState::update(deltaTime);
        if (pacman_->getSprite().getCurrentAnimation()->getName().find("Flashing") == std::string::npos
            && (getTimeout() > ime::Time::Zero && getTimeout() <= ime::seconds(2)))
        {
            pacman_->getSprite().switchAnimation("going" + Utils::convertToString(pacman_->getDirection()) + "Flashing");
            isPacmanFlashing_ = true;
        } else if (isPacmanFlashing_ && getTimeout() > ime::seconds(2)) {
            pacman_->getSprite().switchAnimation("going" + Utils::convertToString(pacman_->getDirection()) + "Super");
            isPacmanFlashing_ = false;
        }
    }

    void SuperState::onExit() {
        pacman_->setSpeed(Constants::PacManNormalSpeed);
        pacman_->setVulnerable(true);
        triggerAnimationSwitch();
    }

    void SuperState::triggerAnimationSwitch() {
        //Trigger animation switch in current direction (Pacman will play normal size animations when vulnerable)
        auto prevDir = pacman_->getDirection();
        pacman_->setDirection(ime::Direction::Unknown);
        pacman_->setDirection(prevDir);

        //Accommodate change in sprite size
        pacman_->getSprite().setOrigin(pacman_->getSprite().getLocalBounds().width / 2.0f,
            pacman_->getSprite().getLocalBounds().height / 2.0f);
    }

    void SuperState::onTimeout() {
        pacman_->popState();
        callback();
    }
}