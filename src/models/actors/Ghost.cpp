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

#include "src/models/actors/Ghost.h"
#include "src/utils/Utils.h"
#include "src/animations/GhostAnimations.h"
#include "src/models/actors/states/ghost/GIdleState.h"
#include "src/common/Constants.h"
#include <memory>
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    Ghost::Ghost(ime::Scene& scene, Colour colour) :
        ime::GameObject(scene),
        gridMover_{nullptr},
        direction_{ime::Left},
        isPacmanSuper_{false}
    {
        setCollisionGroup("ghosts");

        auto animations = GhostAnimations();
        int spriteSheetRow;
        if (colour == Colour::Red) {
            setTag("blinky");
            spriteSheetRow = 0;
        } else if (colour == Colour::Pink) {
            setTag("pinky");
            spriteSheetRow = 1;
        } else if (colour == Colour::Cyan) {
            setTag("inky");
            spriteSheetRow = 2;
        } else if (colour == Colour::Orange) {
            setTag("clyde");
            spriteSheetRow = 3;
        } else {
            assert(false && "Unknown ghost colour");
        }

        // Init default animation
        animations.createAnimationsFor(getTag());
        getSprite() = animations.getAll().at(0)->getSpriteSheet().getSprite(ime::Index{spriteSheetRow, 0});
        resetSpriteOrigin();
        for (const auto& animation : animations.getAll())
            getSprite().getAnimator().addAnimation(animation);

        getSprite().scale(2.0f, 2.0f);
        getSprite().getAnimator().startAnimation("goingLeft");
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::initFSM() {
        fsm_.clear();

        auto initialState = std::make_unique<GIdleState>(&fsm_);
        initialState->setTarget(this);
        fsm_.push(std::move(initialState));
        fsm_.start();
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::setMovementController(ime::GridMover* gridMover) {
        gridMover_ = gridMover;
        gridMover_->setTarget(this);

        // Trigger animation switch when the ghost changes direction
        gridMover_->onAdjacentMoveBegin([this](ime::Index) {
            // Update animation
            direction_ = gridMover_->getDirection();

            // Evade/frightened animation is the same in all directions
            if (getState() == State::Evade)
                return;

            auto newAnimation = "going" + utils::convertToString(direction_);

            if (getState() == State::Heal)
                newAnimation += "Eaten";
            else if (isPacmanSuper_)
                newAnimation += "Flat";

            auto& animator = getSprite().getAnimator();
            if (animator.getActiveAnimation()->getName() != newAnimation)
                animator.startAnimation(newAnimation);
        });
    }

    ///////////////////////////////////////////////////////////////
    ime::GridMover* Ghost::getMoveController() const {
        return gridMover_;
    }

    ///////////////////////////////////////////////////////////////
    std::string Ghost::getClassName() const {
        return "Ghost";
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
    void Ghost::update(ime::Time deltaTime) {
        assert(fsm_.top() && "A ghost FSM must have at least one state before it is updated");

        ime::GameObject::update(deltaTime);
        fsm_.top()->update(deltaTime);
    }

    ///////////////////////////////////////////////////////////////
    void Ghost::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        assert(fsm_.top() && "A ghost FSM must have at least one state before handling an event");

        if (event == GameEvent::SuperModeBegin)
            isPacmanSuper_ = true;
        else if (event == GameEvent::SuperModeEnd)
            isPacmanSuper_ = false;

        fsm_.top()->handleEvent(event, args);
    }

    ///////////////////////////////////////////////////////////////
    bool Ghost::isPacmanSuper() const {
        return isPacmanSuper_;
    }

    ///////////////////////////////////////////////////////////////
    Ghost::~Ghost() = default;

} // namespace spm
