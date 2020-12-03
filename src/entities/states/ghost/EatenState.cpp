#include "EatenState.h"
#include "../../../common/Constants.h"
#include <cassert>

namespace SuperPacMan {
    EatenState::EatenState(std::shared_ptr<IME::Entity> ghost, IME::TileMap &grid) :
        dirChangeHandlerId_{-1},
        ghostMover_(grid, ghost)
    {
        assert(std::dynamic_pointer_cast<Ghost>(ghost) && "Cannot create ghost state for non ghost object");
        ghost_ = std::move(std::dynamic_pointer_cast<Ghost>(ghost));
    }

    void EatenState::onEntry() {
        ghost_->setAlive(false);
        ghost_->setVulnerable(false);
        ghost_->setSpeed(ghost_->getSpeed() * 4.0f);

        ghostMover_.onDestinationReached([this](IME::Graphics::Tile) {
            ghost_->popState();
            //@todo push chase state
        });

        ghostMover_.setDestination(Constants::EatenGhostRespawnTile);
        ghostMover_.startMovement();
    }

    void EatenState::update(float deltaTime) {
        ghostMover_.update(deltaTime);
    }

    void EatenState::onExit() {
        ghostMover_.setTarget(nullptr);
    }
}
