#include "PacManAnimator.h"
#include <IME/core/event/EventDispatcher.h>
#include <cassert>

namespace SuperPacMan {
    PacManAnimator::PacManAnimator(IME::Graphics::AnimatableSprite& sprite, IME::Entity &pacman)
        : pacman_(pacman), pacmanSprite_(sprite), pacmanSize_(Size::Normal), isPacmanDying_(false)
    {
        assert(pacman.getType() == "PacMan" && "Provided entity is not pacman");
    }

    void PacManAnimator::initialize() {
        auto normalFrameSize = IME::Dimensions{16.0f, 16.0f};
        auto superFrameSize = IME::Dimensions{2 * normalFrameSize.width, 2 * normalFrameSize.height};
        auto movementAnimDuration = 0.15f;

        //Pacman animations for going left
        auto goingLeftAnim = std::make_shared<IME::Animation>("goingLeft", "spritesTileset.png", movementAnimDuration);
        goingLeftAnim->addFrames({100, 1}, {normalFrameSize.width, normalFrameSize.height}, 3, 1);
        goingLeftAnim->setLoop(true);
        pacmanSprite_.addAnimation(std::move(goingLeftAnim));

        auto goingLeftSuperAnim = std::make_shared<IME::Animation>("goingLeftSuper", "spritesTileset.png", movementAnimDuration);
        goingLeftSuperAnim->addFrames({1, 1}, {superFrameSize.width, superFrameSize.height}, 3, 1);
        goingLeftSuperAnim->setLoop(true);
        pacmanSprite_.addAnimation(std::move(goingLeftSuperAnim));

        //Pacman animations for going right
        auto goingRightAnim = std::make_shared<IME::Animation>("goingRight", "spritesTileset.png", movementAnimDuration);
        goingRightAnim->addFrames({100, 35}, {normalFrameSize.width, normalFrameSize.height}, 3, 1);
        goingRightAnim->setLoop(true);
        pacmanSprite_.addAnimation(std::move(goingRightAnim));

        auto goingRightSuperAnim = std::make_shared<IME::Animation>("goingRightSuper", "spritesTileset.png", movementAnimDuration);
        goingRightSuperAnim->addFrames({1, 67}, {superFrameSize.width, superFrameSize.height}, 3, 1);
        goingRightSuperAnim->setLoop(true);
        pacmanSprite_.addAnimation(std::move(goingRightSuperAnim));

        //Pacman animations for going up
        auto goingUpAnim = std::make_shared<IME::Animation>("goingUp", "spritesTileset.png", movementAnimDuration);
        goingUpAnim->addFrames({100, 18}, {normalFrameSize.width, normalFrameSize.height}, 3, 1);
        goingUpAnim->setLoop(true);
        pacmanSprite_.addAnimation(std::move(goingUpAnim));

        auto goingUpSuperAnim = std::make_shared<IME::Animation>("goingUpSuper", "spritesTileset.png", movementAnimDuration);
        goingUpSuperAnim->addFrames({1, 34}, {superFrameSize.width, superFrameSize.height}, 3, 1);
        goingUpSuperAnim->setLoop(true);
        pacmanSprite_.addAnimation(std::move(goingUpSuperAnim));

        //Pacman animations for going down
        auto goingDownAnim = std::make_shared<IME::Animation>("goingDown", "spritesTileset.png", movementAnimDuration);
        goingDownAnim->addFrames({100, 52}, {normalFrameSize.width, normalFrameSize.height}, 3, 1);
        goingDownAnim->setLoop(true);
        pacmanSprite_.addAnimation(std::move(goingDownAnim));

        auto goingDownSuperAnim = std::make_shared<IME::Animation>("goingDownSuper", "spritesTileset.png", movementAnimDuration);
        goingDownSuperAnim->addFrames({1, 100}, {superFrameSize.width, superFrameSize.height}, 3, 1);
        goingDownSuperAnim->setLoop(true);
        pacmanSprite_.addAnimation(std::move(goingDownSuperAnim));

        //Pacman death animation
        auto deathAnimation = std::make_shared<IME::Animation>("dying", "spritesTileset.png", 2.0f);
        deathAnimation->addFrames({151, 1}, {normalFrameSize.width, normalFrameSize.height}, 11, 1);
        pacmanSprite_.addAnimation(std::move(deathAnimation));

        using Direction = IME::Direction;
        pacman_.onEvent("directionChanged", IME::Callback<Direction>([this](auto dir) {
            if (isPacmanDying_) //Prevent animation switches when death animation is playing
                return;
            switch (dir) {
                case Direction::None:
                    break;
                case Direction::Left:
                    if (pacmanSize_ == Size::Normal)
                        pacmanSprite_.switchAnimation("goingLeft");
                    else if (pacmanSize_ == Size::SuperSize)
                        pacmanSprite_.switchAnimation("goingLeftSuper");
                    break;
                case Direction::Right:
                    if (pacmanSize_ == Size::Normal)
                        pacmanSprite_.switchAnimation("goingRight");
                    else if (pacmanSize_ == Size::SuperSize)
                        pacmanSprite_.switchAnimation("goingRightSuper");
                    break;
                case Direction::Up:
                    if (pacmanSize_ == Size::Normal)
                        pacmanSprite_.switchAnimation("goingUp");
                    else if (pacmanSize_ == Size::SuperSize)
                        pacmanSprite_.switchAnimation("goingUpSuper");
                    break;
                case Direction::Down:
                    if (pacmanSize_ == Size::Normal)
                        pacmanSprite_.switchAnimation("goingDown");
                    else if (pacmanSize_ == Size::SuperSize)
                        pacmanSprite_.switchAnimation("goingDownSuper");
                    break;
            }
        }));

        //Have the pacman sprite always have the same position as pacman when he moves
        pacman_.onEvent("positionChanged", IME::Callback<float, float>([this](float x, float y) {
            pacmanSprite_.setPosition(x + pacman_.getSize().width / 2.0f, y + pacman_.getSize().height / 2.0f);
        }));

        pacman_.onEvent("killed", IME::Callback<>([this]{
            pacmanSprite_.switchAnimation("dying");
            pacmanSprite_.setOrigin(pacmanSprite_.getSize().width / 2.0f, pacmanSprite_.getSize().height / 2.0f);
        }));

        pacmanSprite_.onAnimationStart("dying", [this]{
            isPacmanDying_ = true;
        });

        pacmanSprite_.onAnimationFinish("dying", [this]{
            isPacmanDying_ = false;
        });

        //Increase the size of pacman sprite when he eats a super pellet
        IME::EventDispatcher::instance()->onEvent("superPelletEaten", IME::Callback<>([this] {
            if (pacmanSize_ == Size::SuperSize)
                return;

            pacmanSize_ = Size::SuperSize;
            //Force an animation switch
            auto pacmanPrevDir = pacman_.getDirection();
            pacman_.setDirection(Direction::None);
            pacman_.setDirection(pacmanPrevDir);
            pacmanSprite_.setOrigin(pacmanSprite_.getSize().width / 2.0f, pacmanSprite_.getSize().height / 2.0f);
        }));

        //Return pacman sprite to normal size when super mode timeouts
        IME::EventDispatcher::instance()->onEvent("superModeTimeout", IME::Callback<>([this] {
            if (pacmanSize_ == Size::Normal)
                return;

            pacmanSize_ = Size::Normal;
            //Animation switches are triggered by direction changes
            auto pacmanPrevDir = pacman_.getDirection();
            pacman_.setDirection(Direction::None);
            pacman_.setDirection(pacmanPrevDir);
            pacmanSprite_.setOrigin(pacmanSprite_.getSize().width / 2.0f, pacmanSprite_.getSize().height / 2.0f);
        }));

        //Set left animation as the starting animation
        pacmanSprite_.switchAnimation("goingLeft");
        pacmanSprite_.setOrigin(pacmanSprite_.getSize().width / 2.0f, pacmanSprite_.getSize().height / 2.0f);
        pacmanSprite_.scale(2.3f, 2.3f);
        pacmanSprite_.setPosition(pacman_.getPosition().x + pacman_.getSize().width / 2.0f,
            pacman_.getPosition().y + pacman_.getSize().height / 2.0f);
    }

    void PacManAnimator::update(float deltaTime) {
        pacmanSprite_.updateAnimation(deltaTime);
    }
}
