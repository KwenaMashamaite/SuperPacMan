#include "PelletAnimations.h"

namespace SuperPacMan {
    void PelletAnimations::createAnimationFor(PelletType pelletType) {
        auto blinkAnimation = std::make_shared<IME::Animation>("blink", "spritesheet.png", 0.3f);
        blinkAnimation->setLoop(true);
        if (pelletType == PelletType::PowerPellet)
            blinkAnimation->addFrames({151, 18}, {16, 16}, 2);
        else if (pelletType == PelletType::SuperPellet) {
            blinkAnimation->setDuration(1.0f);
            blinkAnimation->addFrames({168, 35}, {16, 16}, 6);
        }
        animations_.push_back(std::move(blinkAnimation));
    }

    const std::vector<std::shared_ptr<IME::Animation>> & PelletAnimations::getAll() const {
        return animations_;
    }
}