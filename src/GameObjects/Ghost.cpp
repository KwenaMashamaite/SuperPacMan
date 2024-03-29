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
#include "Common/ObjectReferenceKeeper.h"
#include <memory>
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    Ghost::Ghost(ime::Scene& scene, Colour colour) :
        ime::GridObject(scene),
        isLockedInHouse_{false},
        isFlat_{false}
    {
        setDirection(ime::Right);

        if (colour == Colour::Red)
            setTag("blinky");
        else if (colour == Colour::Pink)
            setTag("pinky");
        else if (colour == Colour::Cyan) {
            setTag("inky");
            setDirection(ime::Left);
        } else if (colour == Colour::Orange)
            setTag("clyde");

        initAnimations();
        ObjectReferenceKeeper::registerGameObject(this);
    }

    ///////////////////////////////////////////////////////////////
    std::string Ghost::getClassName() const {
        return "Ghost";
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::setState(IActorState::Ptr state) {
        if (state_)
            state_->onExit();

        if (state) {
            state_ = std::move(state);
            auto* ghostState = dynamic_cast<GhostState*>(state_.get());
            assert(ghostState && "Invalid ghost state");
            ghostState->setTarget(this);
            state_->onEntry();
        } else
            clearState();
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::clearState() {
        state_ = nullptr;
        ime::GameObject::setState(static_cast<int>(State::None));
    }

    ///////////////////////////////////////////////////////////////
    Ghost::State Ghost::getState() const {
        return static_cast<Ghost::State>(ime::GameObject::getState());
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
                    getSprite().getAnimator().startAnimation("going" + utils::convertToString(getDirection()) + "Flat");
                else
                    getSprite().getAnimator().startAnimation("going" + utils::convertToString(getDirection()));
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    bool Ghost::isFlat() const {
        return isFlat_;
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::update(ime::Time deltaTime) {
        if (state_)
            state_->update(deltaTime);
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        if (state_)
            state_->handleEvent(event, args);
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
        getSprite().getAnimator().startAnimation("going" + utils::convertToString(getDirection()));

        // Automatically change the animation when the direction changes
        onPropertyChange("direction", [this](const ime::Property& property) {
            // Evade/frightened animation is the same in all directions
            if (getState() == State::Frightened)
                return;

            std::string newAnimation = "going" + utils::convertToString(property.getValue<ime::Direction>());

            if (getState() == State::Eaten)
                newAnimation += "Eaten";
            else if (isFlat_)
                newAnimation += "Flat";

            ime::Animator& animator = getSprite().getAnimator();
            if (animator.getActiveAnimation()->getName() != newAnimation)
                animator.startAnimation(newAnimation);
        });
    }

    ///////////////////////////////////////////////////////////////
    Ghost::~Ghost() {
        ObjectReferenceKeeper::deregisterGameObject(this);
    }
} // namespace spm
