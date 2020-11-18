#include "PelletAnimator.h"
#include <cassert>

namespace SuperPacMan {
    PelletAnimator::PelletAnimator(PelletType pelletType,
        IME::Graphics::AnimatableSprite& sprite,
        IME::Entity &pellet)
         : pelletType_(pelletType), pelletSprite_(sprite), pellet_(pellet)
    {
        assert(pellet.getType() == "Pellet" && "Provided entity is not a pellet");
    }

    void PelletAnimator::initialize() {
        auto blinkAnimation = std::make_shared<IME::Animation>("blink", "spritesheet.png", 0.3f);
        blinkAnimation->setLoop(true);
        if (pelletType_ == PelletType::PowerPellet)
            blinkAnimation->addFrames({151, 18}, {16, 16}, 2, 1);
        else {
            blinkAnimation->setDuration(1.0f);
            blinkAnimation->addFrames({168, 35}, {16, 16}, 6, 1);
        }
        pelletSprite_.addAnimation(std::move(blinkAnimation));

        pellet_.onEvent("positionChanged", IME::Callback<float, float>([this](float x, float y) {
            pelletSprite_.setPosition(x + pellet_.getSize().x / 2.0f, y + pellet_.getSize().y / 2.0f);
        }));

        pelletSprite_.switchAnimation("blink");
        pelletSprite_.setOrigin(pelletSprite_.getSize().x / 2.0f, pelletSprite_.getSize().y / 2.0f);
        pelletSprite_.scale(2.0f, 2.0f);
        pelletSprite_.setPosition(pellet_.getPosition().x + pellet_.getSize().x / 2.0f,
            pellet_.getPosition().y + pellet_.getSize().y / 2.0f);
    }

    void PelletAnimator::update(float deltaTime) {
        pelletSprite_.updateAnimation(deltaTime);
    }
}
