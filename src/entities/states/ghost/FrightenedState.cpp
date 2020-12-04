#include "FrightenedState.h"
#include <cassert>

namespace SuperPacMan {
    FrightenedState::FrightenedState(std::shared_ptr<IME::Entity> ghost, IME::TileMap &grid) :
        ghostMover_(grid, ghost)
    {
        assert(std::dynamic_pointer_cast<Ghost>(ghost) && "Cannot create ghost state for non ghost object");
        ghost_ = std::move(std::dynamic_pointer_cast<Ghost>(ghost));
    }

    void FrightenedState::onEntry() {
        ghost_->setVulnerable(true);
        ghost_->setSpeed(ghost_->getSpeed() / 2.0f);
        ghost_->getSprite().switchAnimation("frightened");
        ghostMover_.startMovement();
    }

    void FrightenedState::update(float deltaTime) {
        ghostMover_.update(deltaTime);
    }

    void FrightenedState::onExit() {
        ghostMover_.setTarget(nullptr);
    }
}