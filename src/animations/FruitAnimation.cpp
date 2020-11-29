#include "FruitAnimation.h"

namespace SuperPacMan {
    FruitAnimation::FruitAnimation() {
        animation_ = std::make_shared<IME::Animation>("foodSlide", "spritesheet.png", 1.0f);
        animation_->addFrames({151, 52}, {16, 16}, 17);
        animation_->setLoop(true);
    }

    std::shared_ptr<IME::Animation> FruitAnimation::getAnimation() {
        return animation_;
    }
}
