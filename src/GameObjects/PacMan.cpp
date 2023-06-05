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

#include "PacMan.h"
#include "Animations/PacManAnimations.h"
#include "Utils/Utils.h"
#include "Common/Constants.h"
#include "Common/ObjectReferenceKeeper.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    PacMan::PacMan(mighter2d::Scene& scene) :
        mighter2d::GridObject(scene),
        livesCount_{Constants::PacManLives}
    {
        setTag("pacman");
        getCollisionExcludeList().add("hiddenWall"); // Make pacman able to pass through a special hidden wall
        initAnimations();
        setDirection(mighter2d::Left);
        setState(State::Normal);
        ObjectReferenceKeeper::registerGameObject(this);
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::setLivesCount(int numOfLives) {
        livesCount_ = numOfLives;
        emitChange(mighter2d::Property{"livesCount", livesCount_});
    }

    ///////////////////////////////////////////////////////////////
    int PacMan::getLivesCount() const {
        return livesCount_;
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::addLife() {
        livesCount_++;
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::switchAnimation(mighter2d::Vector2i dir) {
        auto strDir = utils::convertToString(dir);
        auto& animator = getSprite().getAnimator();

        if (getState() != State::Super)
            animator.startAnimation("going" + strDir);
        else if (animator.getActiveAnimation()->getName().find("Flashing") != std::string::npos)
            animator.startAnimation("going" + strDir + "Flashing");
        else
            animator.startAnimation("going" + strDir + "Super");
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::setState(PacMan::State state) {
        mighter2d::GameObject::setState(static_cast<int>(state));
        switchAnimation(getDirection() * -1);
        switchAnimation(getDirection());
    }

    ///////////////////////////////////////////////////////////////
    PacMan::State PacMan::getState() const {
        return static_cast<State>(mighter2d::GameObject::getState());
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::setFlashEnable(bool flash) {
        if (isFlashing() != flash && getState() == State::Super) {
            if (flash)
                getSprite().getAnimator().startAnimation("going" + utils::convertToString(getDirection()) + "Flashing");
            else
                getSprite().getAnimator().startAnimation("going" + utils::convertToString(getDirection()) + "Super");
        }
    }

    ///////////////////////////////////////////////////////////////
    bool PacMan::isFlashing() const {
        return getSprite().getAnimator().getActiveAnimation()->getName().find("Flashing") != std::string::npos;
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::handleEvent(GameEvent event) {
        if (event == GameEvent::SuperModeBegin)
            setState(State::Super);
        else if (event == GameEvent::SuperModeEnd)
            setState(State::Normal);
    }

    ///////////////////////////////////////////////////////////////
    std::string PacMan::getClassName() const {
        return "PacMan";
    }

    ///////////////////////////////////////////////////////////////
    void PacMan::initAnimations() {
        auto animations = PacManAnimations();
        animations.create();

        getSprite().setRenderLayer("pacman");
        auto spritesheet = animations.getAll().at(0)->getSpriteSheet();

        getSprite().setTexture(spritesheet.getTexture());

        for (const auto& animation : animations.getAll())
            getSprite().getAnimator().addAnimation(animation);

        getTransform().scale(2.0f, 2.0f);

        // Automatically switch animations when the direction changes
        onPropertyChange("direction", [this](const mighter2d::Property& property) {
            switchAnimation(property.getValue<mighter2d::Direction>());
        });
    }

    ///////////////////////////////////////////////////////////////
    PacMan::~PacMan() {
        ObjectReferenceKeeper::deregisterGameObject(this);
    }

} // namespace spm
