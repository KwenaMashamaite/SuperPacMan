#include "PacManAnimations.h"
#include <IME/common/Vector2.h>

const auto normalFrameSize = IME::Vector2i{16, 16};
const auto superFrameSize = IME::Vector2i{2 * normalFrameSize.x, 2 * normalFrameSize.y};
const auto movementAnimDuration = 0.15f;

namespace SuperPacMan {

    void PacManAnimations::create() {
        createAnimation("goingLeft", {100, 1, normalFrameSize.x, normalFrameSize.y});
        createAnimation("goingLeftSuper", {1, 1, superFrameSize.x, superFrameSize.y});
        createAnimation("goingRight", {100, 35, normalFrameSize.x, normalFrameSize.y});
        createAnimation("goingRightSuper", {1, 67, superFrameSize.x, superFrameSize.y});
        createAnimation("goingUp", {100, 18, normalFrameSize.x, normalFrameSize.y});
        createAnimation("goingUpSuper", {1, 34, superFrameSize.x, superFrameSize.y});
        createAnimation("goingDown", {100, 52, normalFrameSize.x, normalFrameSize.y});
        createAnimation("goingDownSuper", {1, 100, superFrameSize.x, superFrameSize.y});

        // Death animation
        auto deathAnimation = std::make_shared<IME::Animation>("dying", "spritesheet.png", 2.0f);
        deathAnimation->addFrames({151, 1}, normalFrameSize, 11, 1);
        animations_.push_back(std::move(deathAnimation));
    }

    void PacManAnimations::createAnimation(const std::string &name, IME::IntRect rect) {
        auto anim = std::make_shared<IME::Animation>(name, "spritesheet.png", movementAnimDuration);
        anim->addFrames(rect.getPosition(), rect.getSize(), 3);
        anim->setLoop(true);
        animations_.push_back(std::move(anim));
    }

    const std::vector<std::shared_ptr<IME::Animation>> &PacManAnimations::getAll() const {
        return animations_;
    }
}
