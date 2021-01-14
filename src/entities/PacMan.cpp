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
            sprite_.addAnimation(animation);

        setDirection(ime::Direction::Left);
        sprite_.switchAnimation("goingLeft");
        sprite_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
        sprite_.scale(2.0f, 2.0f);

        onEvent("positionChange", ime::Callback<float, float>([this](float x, float y) {
            sprite_.setPosition(x + getSize().x / 2.0f, y + getSize().y / 2.0f);
        }));

        onEvent("directionChange", ime::Callback<ime::Direction>([this](ime::Direction dir) {
            auto newDir = Utils::convertToString(dir);
            if (stateController_.getCurrentState().first != std::to_string(static_cast<int>(States::Super)))
                sprite_.switchAnimation("going" + newDir);
            else if (sprite_.getCurrentAnimation()->getName().find("Flashing") != std::string::npos)
                sprite_.switchAnimation("going" + newDir + "Flashing");
            else
                sprite_.switchAnimation("going" + newDir + "Super");
        }));
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

    ime::AnimatableSprite &PacMan::getSprite() {
        return sprite_;
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

    void PacMan::update(float deltaTime) {
        assert(stateController_.getCurrentState().second && "Pacman without state");
        if (isMoving()) {
            auto velocity = getSpeed() * deltaTime;
            switch (getDirection()) {
                case ime::Direction::Unknown:
                    break;
                case ime::Direction::Left:
                    setPosition(getPosition().x - velocity, getPosition().y);
                    break;
                case ime::Direction::Right:
                    setPosition(getPosition().x + velocity, getPosition().y);
                    break;
                case ime::Direction::Up:
                    setPosition(getPosition().x, getPosition().y - velocity);
                    break;
                case ime::Direction::Down:
                    setPosition(getPosition().x, getPosition().y + velocity);
                    break;
            }
        }

        stateController_.getCurrentState().second->update(deltaTime);
        sprite_.updateAnimation(deltaTime);
    }
}
