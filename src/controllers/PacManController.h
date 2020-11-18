/**
 * @brief Controls everything pacman can do
 */

#ifndef PACMANCONTROLLER_H
#define PACMANCONTROLLER_H

#include <IME/core/physics/GridMover.h>
#include <IME/core//input/Keyboard.h>
#include "../entity/PacMan.h"
#include "../animators/PacManAnimator.h"


namespace SuperPacMan {
    class PacManController {
    public:
        PacManController(IME::TileMap& tileMap, std::shared_ptr<PacMan> pacMan,
            IME::Graphics::AnimatableSprite& pacmanSprite);

        void handleInput(IME::Input::Keyboard::Key key);
        void update(float deltaTime);


    private:
        IME::GridMover gridMover_;
    };
}

#endif
