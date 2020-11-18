#include "GhostAnimator.h"
#include <IME/core/event/EventDispatcher.h>
#include <cassert>

//TODO - REFACTOR!!!!

namespace SuperPacMan {
    GhostAnimator::GhostAnimator(GhostName ghostName,
     IME::Graphics::AnimatableSprite& sprite, IME::Entity &ghost)
        : ghostName_(ghostName), ghost_(ghost), ghostSprite_(sprite), pacManMode_(PacManMode::Normal),
          prevPacManMode_(PacManMode::Normal)
    {
        assert(ghost.getType() == "Ghost" && "Provided entity is not a ghost");
    }

    void GhostAnimator::initialize() {
        auto frameSize = IME::Vector2i{16, 16};
        auto movementAnimDuration = 0.24f;

        //Normal animations
        auto goingLeftAnim = std::make_shared<IME::Animation>("goingLeft", "spritesheet.png", movementAnimDuration);
        goingLeftAnim->setLoop(true);

        auto goingRightAnim = std::make_shared<IME::Animation>("goingRight", "spritesheet.png", movementAnimDuration);
        goingRightAnim->setLoop(true);

        auto goingUpAnim = std::make_shared<IME::Animation>("goingUp", "spritesheet.png", movementAnimDuration);
        goingUpAnim->setLoop(true);

        auto goingDownAnim = std::make_shared<IME::Animation>("goingDown", "spritesheet.png", movementAnimDuration);
        goingDownAnim->setLoop(true);

        //Flat Animations
        auto goingLeftFlatAnim = std::make_shared<IME::Animation>("goingLeftFlat", "spritesheet.png", movementAnimDuration);
        goingLeftFlatAnim->setLoop(true);

        auto goingRightFlatAnim = std::make_shared<IME::Animation>("goingRightFlat", "spritesheet.png", movementAnimDuration);
        goingRightFlatAnim->setLoop(true);

        auto goingUpFlatAnim = std::make_shared<IME::Animation>("goingUpFlat", "spritesheet.png", movementAnimDuration);
        goingUpFlatAnim->setLoop(true);

        auto goingDownFlatAnim = std::make_shared<IME::Animation>("goingDownFlat", "spritesheet.png", movementAnimDuration);
        goingDownFlatAnim->setLoop(true);

        //Initialize movement animations
        switch (ghostName_) {
            case GhostName::Blinky:
                goingLeftAnim->addFrames({100, 69}, frameSize, 2, 1);
                goingLeftFlatAnim->addFrames({236, 69}, frameSize, 2, 1);

                goingUpAnim->addFrames({134, 69}, frameSize, 2, 1);
                goingUpFlatAnim->addFrames({270, 69}, frameSize, 2, 1);

                goingRightAnim->addFrames({168, 69}, frameSize, 2, 1);
                goingRightFlatAnim->addFrames({304, 69}, frameSize, 2, 1);

                goingDownAnim->addFrames({202, 69}, frameSize, 2, 1);
                goingDownFlatAnim->addFrames({338, 69}, frameSize, 2, 1);
                break;
            case GhostName::Pinky:
                goingLeftAnim->addFrames({100, 86}, frameSize, 2, 1);
                goingLeftFlatAnim->addFrames({236, 86}, frameSize, 2, 1);

                goingUpAnim->addFrames({134, 86}, frameSize, 2, 1);
                goingUpFlatAnim->addFrames({270, 86}, frameSize, 2, 1);

                goingRightAnim->addFrames({168, 86}, frameSize, 2, 1);
                goingRightFlatAnim->addFrames({304, 86}, frameSize, 2, 1);

                goingDownAnim->addFrames({202, 86}, frameSize, 2, 1);
                goingDownFlatAnim->addFrames({338, 86}, frameSize, 2, 1);
                break;
            case GhostName::Inky:
                goingLeftAnim->addFrames({100, 103}, frameSize, 2, 1);
                goingLeftFlatAnim->addFrames({236, 103}, frameSize, 2, 1);

                goingUpAnim->addFrames({134, 103}, frameSize, 2, 1);
                goingUpFlatAnim->addFrames({270, 103}, frameSize, 2, 1);

                goingRightAnim->addFrames({168, 103}, frameSize, 2, 1);
                goingRightFlatAnim->addFrames({304, 103}, frameSize, 2, 1);

                goingDownAnim->addFrames({202, 103}, frameSize, 2, 1);
                goingDownFlatAnim->addFrames({338, 103}, frameSize, 2, 1);
                break;
            case GhostName::Clyde:
                goingLeftAnim->addFrames({100, 120}, frameSize, 2, 1);
                goingLeftFlatAnim->addFrames({236, 120}, frameSize, 2, 1);

                goingUpAnim->addFrames({134, 120}, frameSize, 2, 1);
                goingUpFlatAnim->addFrames({270, 120}, frameSize, 2, 1);

                goingRightAnim->addFrames({168, 120}, frameSize, 2, 1);
                goingRightFlatAnim->addFrames({304, 120}, frameSize, 2, 1);

                goingDownAnim->addFrames({202, 120}, frameSize, 2, 1);
                goingDownFlatAnim->addFrames({339, 120}, frameSize, 2, 1);
                break;
        }

        //Add normal animations
        ghostSprite_.addAnimation(std::move(goingLeftAnim));
        ghostSprite_.addAnimation(std::move(goingRightAnim));
        ghostSprite_.addAnimation(std::move(goingUpAnim));
        ghostSprite_.addAnimation(std::move(goingDownAnim));

        //Add flat animations
        ghostSprite_.addAnimation(std::move(goingLeftFlatAnim));
        ghostSprite_.addAnimation(std::move(goingRightFlatAnim));
        ghostSprite_.addAnimation(std::move(goingUpFlatAnim));
        ghostSprite_.addAnimation(std::move(goingDownFlatAnim));

        //Add frightened animations (The animation is the same for all ghosts)
        auto frightenedAnim = std::make_shared<IME::Animation>("frightened", "spritesheet.png", movementAnimDuration);
        frightenedAnim->addFrames({389, 69}, frameSize, 2, 1);
        frightenedAnim->setLoop(true);
        ghostSprite_.addAnimation(std::move(frightenedAnim));

        //Add eaten animations (The animations are the same for all ghosts)
        auto goingLeftEatenAnim = std::make_shared<IME::Animation>("goingLeftEaten", "spritesheet.png", movementAnimDuration);
        goingLeftEatenAnim->addFrames({389, 86}, frameSize, 2, 1, IME::Arrangement::Vertical);
        goingLeftEatenAnim->setLoop(true);
        ghostSprite_.addAnimation(std::move(goingLeftEatenAnim));

        auto goingRightEatenAnim = std::make_shared<IME::Animation>("goingRightEaten", "spritesheet.png", movementAnimDuration);
        goingRightEatenAnim->addFrames({423, 86}, frameSize, 2, 1, IME::Arrangement::Vertical);
        goingRightEatenAnim->setLoop(true);
        ghostSprite_.addAnimation(std::move(goingRightEatenAnim));

        auto goingUpEatenAnim = std::make_shared<IME::Animation>("goingUpEaten", "spritesheet.png", movementAnimDuration);
        goingUpEatenAnim->addFrames({406, 86}, frameSize, 2, 1, IME::Arrangement::Vertical);
        goingUpEatenAnim->setLoop(true);
        ghostSprite_.addAnimation(std::move(goingUpEatenAnim));

        auto goingDownEatenAnim = std::make_shared<IME::Animation>("goingDownEaten", "spritesheet.png", movementAnimDuration);
        goingDownEatenAnim->addFrames({440, 86}, frameSize, 2, 1, IME::Arrangement::Vertical);
        goingDownEatenAnim->setLoop(true);
        ghostSprite_.addAnimation(std::move(goingDownEatenAnim));

        using Direction = IME::Direction;
        ghost_.onEvent("directionChanged", IME::Callback<Direction>([this](auto dir) {
            //The frightened animation is the same for all directions
            if (pacManMode_ == PacManMode::Energized)
                return;

            switch (dir) {
                case Direction::None:
                    break;
                case Direction::Left:
                    if (!ghost_.isAlive())
                        ghostSprite_.switchAnimation("goingLeftEaten");
                    else if (pacManMode_ == PacManMode::Normal)
                        ghostSprite_.switchAnimation("goingLeft");
                    else if (pacManMode_ == PacManMode::SuperSized)
                        ghostSprite_.switchAnimation("goingLeftFlat");
                    break;
                case Direction::Right:
                    if (!ghost_.isAlive())
                        ghostSprite_.switchAnimation("goingRightEaten");
                    else if (pacManMode_ == PacManMode::Normal)
                        ghostSprite_.switchAnimation("goingRight");
                    else if (pacManMode_ == PacManMode::SuperSized)
                        ghostSprite_.switchAnimation("goingRightFlat");
                    break;
                case Direction::Up:
                    if (!ghost_.isAlive())
                        ghostSprite_.switchAnimation("goingUpEaten");
                    else if (pacManMode_ == PacManMode::Normal)
                        ghostSprite_.switchAnimation("goingUp");
                    else if (pacManMode_ == PacManMode::SuperSized)
                        ghostSprite_.switchAnimation("goingUpFlat");
                    break;
                case Direction::Down:
                    if (!ghost_.isAlive())
                        ghostSprite_.switchAnimation("goingDownEaten");
                    else if (pacManMode_ == PacManMode::Normal)
                        ghostSprite_.switchAnimation("goingDown");
                    else if (pacManMode_ == PacManMode::SuperSized)
                        ghostSprite_.switchAnimation("goingDownFlat");
                    break;
            }
        }));

        //Have the ghost sprite track the position of the ghost
        ghost_.onEvent("positionChanged", IME::Callback<float, float>([this](float x, float y) {
            ghostSprite_.setPosition(x + ghost_.getSize().x / 2.0f, y + ghost_.getSize().y / 2.0f);
        }));

        //Reset energized mode when a ghost is eaten by pacman
        ghost_.onEvent("killed", IME::Callback<>([this]{
            pacManMode_ = prevPacManMode_; //Reset energized mode
            auto ghostPrevDir = ghost_.getDirection();
            ghost_.setDirection(Direction::None);
            ghost_.setDirection(ghostPrevDir);
        }));

        //
        ghost_.onEvent("revived", IME::Callback<>([this]{
            auto ghostPrevDir = ghost_.getDirection();
            ghost_.setDirection(Direction::None);
            ghost_.setDirection(ghostPrevDir);
        }));

        //Frighten ghost when pacman eats an energizer pellet
        IME::EventDispatcher::instance()->onEvent("energizerEaten", IME::Callback<>([this] {
            if (pacManMode_ == PacManMode::Energized)
                return;

            prevPacManMode_ = pacManMode_;
            pacManMode_ = PacManMode::Energized;
            ghostSprite_.switchAnimation("frightened");
        }));

        //Unfrighten ghost when pacman eats an energizer pellet
        IME::EventDispatcher::instance()->onEvent("energizedModeTimeout", IME::Callback<>([this] {
            if (pacManMode_ == PacManMode::Energized) {
                pacManMode_ = prevPacManMode_;
                auto ghostPrevDir = ghost_.getDirection();
                ghost_.setDirection(Direction::None);
                ghost_.setDirection(ghostPrevDir);
            }
        }));

        //Flatten ghost when pacman eats super pellet
        IME::EventDispatcher::instance()->onEvent("superPelletEaten", IME::Callback<>([this] {
            if (pacManMode_ == PacManMode::SuperSized)
                return;
            else if (pacManMode_ == PacManMode::Energized) {
                prevPacManMode_ = PacManMode::SuperSized;
                return;
            }

            pacManMode_ = PacManMode::SuperSized;
            auto ghostPrevDir = ghost_.getDirection();
            ghost_.setDirection(Direction::None);
            ghost_.setDirection(ghostPrevDir);
        }));

        //Return ghosts to normal when super mode timeouts
        IME::EventDispatcher::instance()->onEvent("superModeTimeout", IME::Callback<>([this] {
            if (pacManMode_ == PacManMode::SuperSized)
                pacManMode_ = PacManMode::Normal;
            else if (pacManMode_ == PacManMode::Energized)
                prevPacManMode_ = PacManMode::Normal;

            auto ghostPrevDir = ghost_.getDirection();
            ghost_.setDirection(Direction::None);
            ghost_.setDirection(ghostPrevDir);
        }));

        //Set left animation as default animation
        ghostSprite_.switchAnimation("goingLeft");
        ghostSprite_.setOrigin(ghostSprite_.getSize().x / 2.0f, ghostSprite_.getSize().y / 2.0f);
        ghostSprite_.setPosition(ghost_.getPosition().x + ghost_.getSize().x / 2.0f,
            ghost_.getPosition().y + ghost_.getSize().y / 2.0f);
        ghostSprite_.scale(2.0f, 2.0f);
    }

    void GhostAnimator::reset() {
        pacManMode_ = PacManMode::Normal;
        auto ghostPrevDir = ghost_.getDirection();
        ghost_.setDirection(IME::Direction::None);
        ghost_.setDirection(ghostPrevDir);
    }

    void GhostAnimator::update(float deltaTime) {
        ghostSprite_.updateAnimation(deltaTime);
    }
}
