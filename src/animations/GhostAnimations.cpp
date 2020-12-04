#include "GhostAnimations.h"
#include <IME/common/Vector2.h>

//The size of each animation frame
const auto frameSize = IME::Vector2i{16, 16};
//The duration of each movement animation
const auto movementAnimDuration = 0.18f;

namespace SuperPacMan {
    void GhostAnimations::createAnimationsFor(Ghost::Name ghostName) {
        if (!animations_.empty())
            animations_.clear();

        createMovementAnimations(ghostName);
        createEatenAnimations();
        createFrightenedAnimations();
    }

    void GhostAnimations::createMovementAnimations(Ghost::Name ghostName) {
        switch (ghostName) {
            case Ghost::Name::Blinky:
                createAnimation("goingLeft", {100, 69});
                createAnimation("goingLeftFlat", {236, 69});
                createAnimation("goingRight", {168, 69});
                createAnimation("goingRightFlat", {304, 69});
                createAnimation("goingUp", {134, 69});
                createAnimation("goingUpFlat", {270, 69});
                createAnimation("goingDown", {202, 69});
                createAnimation("goingDownFlat", {338, 69});
                break;
            case Ghost::Name::Pinky:
                createAnimation("goingLeft", {100, 86});
                createAnimation("goingLeftFlat", {236, 86});
                createAnimation("goingRight", {168, 86});
                createAnimation("goingRightFlat", {304, 86});
                createAnimation("goingUp", {134, 86});
                createAnimation("goingUpFlat", {270, 86});
                createAnimation("goingDown", {202, 86});
                createAnimation("goingDownFlat", {338, 86});
                break;
            case Ghost::Name::Inky:
                createAnimation("goingLeft", {100, 103});
                createAnimation("goingLeftFlat", {236, 103});
                createAnimation("goingRight", {168, 103});
                createAnimation("goingRightFlat", {304, 103});
                createAnimation("goingUp", {134, 103});
                createAnimation("goingUpFlat", {270, 103});
                createAnimation("goingDown", {202, 103});
                createAnimation("goingDownFlat", {338, 103});
                break;
            case Ghost::Name::Clyde:
                createAnimation("goingLeft", {100, 120});
                createAnimation("goingLeftFlat", {236, 120});
                createAnimation("goingRight", {168, 120});
                createAnimation("goingRightFlat", {304, 120});
                createAnimation("goingUp", {134, 120});
                createAnimation("goingUpFlat", {270, 120});
                createAnimation("goingDown", {202, 120});
                createAnimation("goingDownFlat", {338, 120});
                break;
        }
    }

    void GhostAnimations::createFrightenedAnimations() {
        createAnimation("frightened", {389, 69});
        createAnimation("flash", {389, 120});
    }

    void GhostAnimations::createEatenAnimations() {
        createAnimation("goingLeftEaten", {389, 86}, IME::Arrangement::Vertical);
        createAnimation("goingRightEaten", {423, 86}, IME::Arrangement::Vertical);
        createAnimation("goingUpEaten", {406, 86}, IME::Arrangement::Vertical);
        createAnimation("goingDownEaten", {440, 86}, IME::Arrangement::Vertical);
    }

    void GhostAnimations::createAnimation(const std::string &name, IME::Vector2i startPos, IME::Arrangement arrangement) {
        auto anim = std::make_shared<IME::Animation>(name, "spritesheet.png", movementAnimDuration);
        anim->setLoop(true);
        anim->addFrames(startPos, frameSize, 2, 1, arrangement);
        animations_.push_back(std::move(anim));
    }

    const std::vector<std::shared_ptr<IME::Animation>> &GhostAnimations::getAll() const {
        return animations_;
    }
}
