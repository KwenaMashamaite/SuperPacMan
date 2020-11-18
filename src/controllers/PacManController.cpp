#include "PacManController.h"

namespace SuperPacMan {
    PacManController::PacManController(IME::TileMap &tileMap,
        std::shared_ptr<PacMan> pacMan,
        IME::Graphics::AnimatableSprite &pacmanSprite)
        : gridMover_(tileMap, std::move(pacMan))
    {}

    void PacManController::handleInput(IME::Input::Keyboard::Key key) {
        /*if (key == IME::Input::Keyboard::Key::Left && gridMover_.requestDirectionChange(IME::Direction::Left))
            animator_.switchAnimation("goingLeft");
        else if (key == IME::Input::Keyboard::Key::Right && gridMover_.requestDirectionChange(IME::Direction::Right))
            animator_.switchAnimation("goingRight");
        else if (key == IME::Input::Keyboard::Key::Up && gridMover_.requestDirectionChange(IME::Direction::Up))
            animator_.switchAnimation("goingUp");
        else if (key == IME::Input::Keyboard::Key::Down && gridMover_.requestDirectionChange(IME::Direction::Down))
            animator_.switchAnimation("goingDown");*/
    }

    void PacManController::update(float deltaTime) {
        gridMover_.update(deltaTime);
        //animator_.update(deltaTime);
    }
}