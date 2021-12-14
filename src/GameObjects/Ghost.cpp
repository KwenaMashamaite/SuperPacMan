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

#include "Ghost.h"
#include "Animations/GhostAnimations.h"
#include "AI/ghost/ScatterState.h"
#include "Utils/Utils.h"
#include <memory>
#include <Common/ObjectReferenceKeeper.h>

namespace spm {
    ///////////////////////////////////////////////////////////////
    Ghost::Ghost(ime::Scene& scene, Colour colour) :
        ime::GameObject(scene),
        direction_{ime::Left},
        isLockedInHouse_{false},
        isFlat_{false}
    {
        if (colour == Colour::Red)
            setTag("blinky");
        else if (colour == Colour::Pink)
            setTag("pinky");
        else if (colour == Colour::Cyan)
            setTag("inky");
        else if (colour == Colour::Orange)
            setTag("clyde");

        initAnimations();
        ObjectReferenceKeeper::registerGameObject(this);
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::initFSM() {
        fsm_.clear();
        fsm_.push(std::make_unique<ScatterState>(&fsm_, this));
        fsm_.start();
    }

    ///////////////////////////////////////////////////////////////
    std::string Ghost::getClassName() const {
        return "Ghost";
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::setState(Ghost::State state) {
        ime::GameObject::setState(static_cast<int>(state));
    }

    ///////////////////////////////////////////////////////////////
    Ghost::State Ghost::getState() const {
        return static_cast<Ghost::State>(ime::GameObject::getState());
    }

    ///////////////////////////////////////////////////////////////
    ime::Vector2i Ghost::getDirection() const {
        return direction_;
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::setLockInGhostHouse(bool lock) {
        isLockedInHouse_ = lock;
    }

    ///////////////////////////////////////////////////////////////
    bool Ghost::isLockedInGhostHouse() const {
        return isLockedInHouse_;
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::setFlash(bool flash) {
        if (isFlashing() != flash && getState() == State::Frightened) {
            if (flash)
                getSprite().getAnimator().startAnimation("flash");
            else
                getSprite().getAnimator().startAnimation("frightened");
        }
    }

    ///////////////////////////////////////////////////////////////
    bool Ghost::isFlashing() const {
        return getSprite().getAnimator().getActiveAnimation()->getName() == "flash";
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::setFlattened(bool flatten) {
        if (isFlat_ != flatten) {
            isFlat_ = flatten;

            if (getState() != State::Eaten) {
                if (flatten)
                    getSprite().getAnimator().startAnimation("going" + utils::convertToString(direction_) + "Flat");
                else
                    getSprite().getAnimator().startAnimation("going" + utils::convertToString(direction_));
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    bool Ghost::isFlat() const {
        return isFlat_;
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::setDirection(ime::Vector2i dir) {
        if (direction_ != dir) {
            direction_ = dir;

            // Evade/frightened animation is the same in all directions
            if (getState() == State::Frightened)
                return;

            auto newAnimation = "going" + utils::convertToString(direction_);

            if (getState() == State::Eaten)
                newAnimation += "Eaten";
            else if (isFlat_)
                newAnimation += "Flat";

            auto& animator = getSprite().getAnimator();
            if (animator.getActiveAnimation()->getName() != newAnimation)
                animator.startAnimation(newAnimation);
        }
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::update(ime::Time deltaTime) {
        fsm_.update(deltaTime);
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        fsm_.handleEvent(event, args);
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::initAnimations() {
        GhostAnimations animations;
        animations.createAnimationsFor(getTag());

        int spriteSheetRow = getTag() == "blinky" ? 0 : (getTag() == "pinky" ? 1 : (getTag() == "inky" ? 2 : 3));
        getSprite() = animations.getAll().at(0)->getSpriteSheet().getSprite(ime::Index{spriteSheetRow, 0});
        for (const auto& animation : animations.getAll())
            getSprite().getAnimator().addAnimation(animation);

        getSprite().scale(2.0f, 2.0f);
        resetSpriteOrigin();
        getSprite().getAnimator().startAnimation("going" + utils::convertToString(direction_));
    }

    ///////////////////////////////////////////////////////////////
    Ghost::~Ghost() {
        ObjectReferenceKeeper::deregisterGameObject(this);
    }
} // namespace spm
