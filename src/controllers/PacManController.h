/**
 * @brief Controls everything pacman can do
 */

#ifndef SUPERPACMAN_PACMANCONTROLLER_H
#define SUPERPACMAN_PACMANCONTROLLER_H

#include <IME/core/physics/GridMover.h>
#include <IME/core//input/Keyboard.h>
#include "../entities/PacMan.h"


namespace pacman {
    class PacManController {
    public:
        PacManController(ime::TileMap& tileMap, std::shared_ptr<PacMan> pacMan,
            ime::AnimatableSprite& pacmanSprite);

        void handleInput(ime::input::Keyboard::Key key);
        void update(float deltaTime);


    private:
        ime::GridMover gridMover_;
    };
}

#endif
