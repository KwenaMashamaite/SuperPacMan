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

#include "Ghost.h"
#include "../animations/GhostAnimations.h"
#include "../common/Constants.h"
#include "../utils/Utils.h"
#include <utility>
#include <cassert>

namespace SuperPacMan {
    Ghost::Ghost(Name ghostName, const IME::Vector2u &boundingRect) :
        Entity(boundingRect, IME::Entity::Type::Enemy),
        ghostName_(ghostName),
        isMoving_(false),
        speed_(Constants::GhostScatterSpeed),
        isFlattened_{false}
    {
        setCollidable(true);
        auto animations = GhostAnimations();
        animations.createAnimationsFor(ghostName);
        for (const auto& animation : animations.getAll())
            sprite_.addAnimation(animation);

        setDirection(IME::Direction::Left);
        sprite_.switchAnimation("goingLeft");
        sprite_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
        sprite_.scale(2.0f, 2.0f);

        //Make the sprite track the position of the ghost
        onEvent("positionChange", IME::Callback<float, float>([this](float x, float y) {
            sprite_.setPosition(x + getSize().x / 2.0f, y + getSize().y / 2.0f);
        }));

        onEvent("directionChange", IME::Callback<IME::Direction>([this](IME::Direction newDir) {
            if (getState().first == States::Frightened || newDir == IME::Direction::Unknown)
                return;

            auto dir = Utils::convertToString(newDir);
            if (getState().first == States::Eaten)
                sprite_.switchAnimation("going" + dir + "Eaten");
            else if (isFlattened_)
                sprite_.switchAnimation("going" + dir + "Flat");
            else
                sprite_.switchAnimation("going" + dir);
        }));
    }

    void Ghost::setSpeed(float speed) {
        if (speed < 0.0f)
            speed_ = 0.0f;
        else
            speed_ = speed;
    }

    float Ghost::getSpeed() const {
        return speed_;
    }

    std::string Ghost::getClassType() {
        return "Ghost";
    }

    Ghost::Name Ghost::getGhostName() const {
        return ghostName_;
    }

    IME::Graphics::AnimatableSprite &Ghost::getSprite() {
        return sprite_;
    }

    void Ghost::pushState(States curState, std::shared_ptr<IState> state) {
        stateController_.pushState(std::to_string(static_cast<int>(curState)), std::move(state));
    }

    std::pair<Ghost::States, std::shared_ptr<IState>> Ghost::getState() {
        if (stateController_.isEmpty())
            return {States::Unknown, nullptr};
        return {static_cast<States>(std::stoi(stateController_.getCurrentState().first)), stateController_.getCurrentState().second};
    }

    void Ghost::popState() {
        stateController_.popState();
    }

    void Ghost::flatten() {
        if (isActive() && getState().first != States::Eaten
            && getState().first != States::Frightened)
        {
            isFlattened_ = true;
            // Animation switch is triggered by a direction change, check
            // "directionChange" handler defined in constructor
            auto prevDir = getDirection();
            setDirection(IME::Direction::Unknown);
            setDirection(prevDir);
        }
    }

    void Ghost::unflatten() {
        isFlattened_ = false;
    }

    void Ghost::move() {
        if (speed_ > 0.0f && getState().first != States::Idle)
            isMoving_ = true;
    }

    bool Ghost::isMoving() const {
        return isMoving_;
    }

    void Ghost::stop() {
        isMoving_ = false;
    }

    void Ghost::update(float deltaTime) {
        assert(stateController_.getCurrentState().second && "Ghost without state");
        if (isMoving_) {
            auto velocity = getSpeed() * deltaTime;
            switch (getDirection()) {
                case IME::Direction::Unknown:
                    break;
                case IME::Direction::Left:
                    setPosition(getPosition().x - velocity, getPosition().y);
                    break;
                case IME::Direction::Right:
                    setPosition(getPosition().x + velocity, getPosition().y);
                    break;
                case IME::Direction::Up:
                    setPosition(getPosition().x, getPosition().y - velocity);
                    break;
                case IME::Direction::Down:
                    setPosition(getPosition().x, getPosition().y + velocity);
                    break;
            }
        }

        stateController_.getCurrentState().second->update(deltaTime);
        sprite_.updateAnimation(deltaTime);
    }
}
