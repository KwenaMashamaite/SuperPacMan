#include "SuperState.h"
#include "../../../common/Constants.h"

namespace SuperPacMan {
    SuperState::SuperState(std::shared_ptr<IME::Entity> pacman) {
        assert(std::dynamic_pointer_cast<PacMan>(pacman) && "Cannot create Pacman state for non Pacman object");
        pacman_ = std::move(std::dynamic_pointer_cast<PacMan>(pacman));
    }

    void SuperState::onEntry() {
        pacman_->setSpeed(Constants::PacManSuperSpeed);
        pacman_->setVulnerable(false);
        triggerAnimationSwitch();
    }

    void SuperState::update(float deltaTime) {

    }

    void SuperState::onExit() {
        pacman_->setSpeed(Constants::PacManNormalSpeed);
        pacman_->setVulnerable(true);
        triggerAnimationSwitch();
    }

    void SuperState::triggerAnimationSwitch() {
        //Trigger animation switch in current direction (Pacman will play normal size animations when vulnerable)
        auto prevDir = pacman_->getDirection();
        pacman_->setDirection(IME::Direction::None);
        pacman_->setDirection(prevDir);

        //Accommodate change in sprite size
        pacman_->getSprite().setOrigin(pacman_->getSprite().getLocalBounds().width / 2.0f,
            pacman_->getSprite().getLocalBounds().height / 2.0f);
    }
}