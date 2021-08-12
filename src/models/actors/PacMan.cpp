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

#include "src/models/actors/PacMan.h"
#include "src/animations/PacManAnimations.h"
#include "src/utils/Utils.h"
#include "src/common/Constants.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    PacMan::PacMan(ime::Scene& scene) :
        ime::GameObject(scene),
        livesCount_{Constants::PacManLives}
    {
        setTag("pacman");
        setCollisionGroup("pacman");

        // Make pacman able to pass through a special hidden wall
        getCollisionExcludeList().add("hiddenWall");

        // Make pacman not detectable by the slow lane sensor
        getCollisionExcludeList().add("slowLaneEntrySensor");
        getCollisionExcludeList().add("slowLaneExitSensor");

        auto animations = PacManAnimations();
        animations.create();
        getSprite() = animations.getAll().at(0)->getSpriteSheet().getSprite(ime::Index{0, 0});
        for (const auto& animation : animations.getAll())
            getSprite().getAnimator().addAnimation(animation);

        getTransform().scale(2.0f, 2.0f);
        setDirection(ime::Left);
        setState(State::Idle);
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::setLivesCount(int numOfLives) {
        livesCount_ = numOfLives;
        emitChange(ime::Property{"livesCount", livesCount_});
    }

    ///////////////////////////////////////////////////////////////
    int PacMan::getLivesCount() const {
        return livesCount_;
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::switchAnimation(ime::Vector2i dir) {
        auto strDir = utils::convertToString(dir);
        auto& animator = getSprite().getAnimator();

        if (getState() != State::Super)
            animator.startAnimation("going" + strDir);
        else if (animator.getActiveAnimation()->getName().find("Flashing") != std::string::npos)
            animator.startAnimation("going" + strDir + "Flashing");
        else
            animator.startAnimation("going" + strDir + "Super");

        resetSpriteOrigin();
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::setState(PacMan::State state) {
        ime::GameObject::setState(static_cast<int>(state));
        switchAnimation(direction_ * -1);
        switchAnimation(direction_);
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        if (event == GameEvent::SuperModeBegin)
            setState(State::Super);
        else if (event == GameEvent::SuperModeEnd || event == GameEvent::LevelStarted)
            setState(State::Normal);
    }

    ///////////////////////////////////////////////////////////////
    PacMan::State PacMan::getState() const {
        return static_cast<State>(ime::GameObject::getState());
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::setDirection(const ime::Vector2i &direction) {
        if (direction_ != direction) {
            direction_ = direction;
            switchAnimation(direction_);
        }
    }

    ///////////////////////////////////////////////////////////////
    ime::Vector2i PacMan::getDirection() const {
        return direction_;
    }

    ///////////////////////////////////////////////////////////////
    std::string PacMan::getClassName() const {
        return "PacMan";
    }

} // namespace spm
