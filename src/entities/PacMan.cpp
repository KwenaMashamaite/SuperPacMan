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

#include "PacMan.h"
#include "../utils/Utils.h"
#include "../common/Constants.h"
#include "../animations/PacManAnimations.h"
#include <cassert>

namespace pacman {
    PacMan::PacMan(const ime::Vector2u &boundingRect) :
        Entity(boundingRect, ime::Entity::Type::Player),
        numberOfLives_(Constants::PacManLives),
        speed_(Constants::PacManNormalSpeed),
        isMoving_(false)
    {
        setCollidable(true);
        auto animations = PacManAnimations();
        animations.create();
        for (const auto& animation : animations.getAll())
            getSprite().getAnimator().addAnimation(animation);

        //getTransform().scale(2.0f, 2.0f);

        onEvent("directionChange", ime::Callback<ime::Direction>([this](ime::Direction dir) {
            auto newDir = Utils::convertToString(dir);
            if (stateController_.getCurrentState().first != std::to_string(static_cast<int>(States::Super)))
                getSprite().getAnimator().startAnimation("going" + newDir);
            else if (getSprite().getAnimator().getCurrentAnimation()->getName().find("Flashing") != std::string::npos)
                getSprite().getAnimator().startAnimation("going" + newDir + "Flashing");
            else
                getSprite().getAnimator().startAnimation("going" + newDir + "Super");
        }));

        setDirection(ime::Direction::Left);
    }

    void PacMan::setNumberOfLives(unsigned int numOfLives) {
        numberOfLives_ = numOfLives;
        if (numberOfLives_ <= 0 && isActive() && isVulnerable())
            setActive(false);
    }

    unsigned int PacMan::getNumberOfLives() const {
        return numberOfLives_;
    }

    void PacMan::setSpeed(float speed) {
        if (speed < 0.0f)
            speed_ = 0.0f;
        else
            speed_ = speed;
    }

    float PacMan::getSpeed() const {
        return speed_;
    }

    std::string PacMan::getClassType() {
        return "PacMan";
    }

    void PacMan::pushState(PacMan::States curState, std::shared_ptr<IState> state) {
        stateController_.pushState(std::to_string(static_cast<int>(curState)), std::move(state));
        for (auto& callback : stateChangeListeners_)
            if (callback)
                callback(curState);
    }

    std::pair<PacMan::States, std::shared_ptr<IState>> PacMan::getState() {
        if (stateController_.isEmpty())
            return {States::Unknown, nullptr};
        return {static_cast<States>(std::stoi(stateController_.getCurrentState().first)), stateController_.getCurrentState().second};
    }

    void PacMan::popState() {
        stateController_.popState();
        if (!stateController_.isEmpty())
            for (auto& callback : stateChangeListeners_)
                if (callback)
                    callback(getState().first);
    }

    void PacMan::onStateChange(std::function<void(States)> callback) {
        stateChangeListeners_.push_back(std::move(callback));
    }

    void PacMan::move() {
        if (speed_ > 0.0f && getState().first != States::Idle)
            isMoving_ = true;
    }

    bool PacMan::isMoving() const {
        return isMoving_;
    }

    void PacMan::stop() {
        isMoving_ = false;
    }

    void PacMan::update(ime::Time deltaTime) {
        assert(stateController_.getCurrentState().second && "Pacman without state");
        if (isMoving()) {
            auto velocity = getSpeed() * deltaTime.asSeconds();
            switch (getDirection()) {
                case ime::Direction::Unknown:
                    break;
                case ime::Direction::Left:
                    getTransform().move(-velocity, 0);
                    break;
                case ime::Direction::Right:
                    getTransform().move(velocity, 0);
                    break;
                case ime::Direction::Up:
                    getTransform().move(0, -velocity);
                    break;
                case ime::Direction::Down:
                    getTransform().move(0, velocity);
                    break;
            }
        }

        stateController_.getCurrentState().second->update(deltaTime);
        getSprite().updateAnimation(deltaTime);
    }
}
