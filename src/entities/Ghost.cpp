#include "Ghost.h"
#include "../animations/GhostAnimations.h"
#include "../common/Constants.h"
#include <utility>

namespace SuperPacMan {
    Ghost::Ghost(Name ghostName, const IME::Vector2u &boundingRect) :
        Entity(boundingRect, IME::Entity::Type::Enemy),
        ghostName_(ghostName),
        isMoving_(false),
        speed_(Constants::GhostScatterSpeed)
    {
        setCollidable(true);
        auto animations = GhostAnimations();
        animations.createAnimationsFor(ghostName);
        for (const auto& animation : animations.getAll())
            sprite_.addAnimation(animation);

        setDirection(IME::Direction::Left);
        sprite_.switchAnimation("goingLeft");
        sprite_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
        sprite_.scale(2.0f, 2.0f);
        sprite_.setPosition(getSize().x / 2.0f, getSize().y / 2.0f);

        //Make the sprite track the position of the ghost
        onEvent("positionChanged", IME::Callback<float, float>([this](float x, float y) {
            sprite_.setPosition(x + getSize().x / 2.0f, y + getSize().y / 2.0f);
        }));
    }

    std::string Ghost::getClassType() {
        return "Ghost";
    }

    Ghost::Name Ghost::getGhostName() const {
        return ghostName_;
    }

    void Ghost::update(float deltaTime) {
        if (isMoving_) {
            auto velocity = getSpeed() * deltaTime;
            switch (getDirection()) {
                case IME::Direction::None:
                    break;
                case IME::Direction::Left:
                    setPosition(getPosition().x - velocity, getPosition().y);
                    break;
                case IME::Direction::Right:
                    setPosition(getPosition().x + velocity, getPosition().y);
                    break;
                case IME::Direction::Up:
                    setPosition(getPosition().x, getPosition().y - velocity);
                    break;
                case IME::Direction::Down:
                    setPosition(getPosition().x, getPosition().y + velocity);
                    break;
            }
        }

        if (stateController_.getCurrentState())
            stateController_.getCurrentState()->update(deltaTime);

        sprite_.updateAnimation(deltaTime);
    }

    void Ghost::move() {
        if (speed_ > 0.0f)
            isMoving_ = true;
    }

    void Ghost::pushState(std::shared_ptr<IState> state) {
        stateController_.pushState(std::move(state));
    }

    void Ghost::popState() {
        stateController_.popState();
    }

    void Ghost::setSpeed(float speed) {
        if (speed < 0.0f)
            speed_ = 0.0f;
        else
            speed_ = speed;
    }

    float Ghost::getSpeed() const {
        return speed_;
    }

    bool Ghost::isMoving() const {
        return isMoving_;
    }

    void Ghost::stop() {
        isMoving_ = false;
    }

    IME::Graphics::AnimatableSprite &Ghost::getSprite() {
        return sprite_;
    }
}
