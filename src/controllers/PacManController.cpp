#include "PacManController.h"

namespace pacman {
    PacManController::PacManController(ime::TileMap &tileMap,
        std::shared_ptr<PacMan> pacMan,
        ime::AnimatableSprite &pacmanSprite)
        : gridMover_(tileMap, std::move(pacMan))
    {}

    void PacManController::handleInput(ime::input::Keyboard::Key key) {
        /*if (key == ime::input::Keyboard::Key::Left && gridMover_.requestDirectionChange(ime::Direction::Left))
            animator_.switchAnimation("goingLeft");
        else if (key == ime::input::Keyboard::Key::Right && gridMover_.requestDirectionChange(ime::Direction::Right))
            animator_.switchAnimation("goingRight");
        else if (key == ime::input::Keyboard::Key::Up && gridMover_.requestDirectionChange(ime::Direction::Up))
            animator_.switchAnimation("goingUp");
        else if (key == ime::input::Keyboard::Key::Down && gridMover_.requestDirectionChange(ime::Direction::Down))
            animator_.switchAnimation("goingDown");*/
    }

    void PacManController::update(float deltaTime) {
        gridMover_.update(deltaTime);
        //animator_.update(deltaTime);
    }
}