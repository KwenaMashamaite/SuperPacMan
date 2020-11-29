#include "PacMan.h"
#include "../common/Constants.h"
#include "../animations/PacManAnimations.h"

namespace SuperPacMan {
    PacMan::PacMan(const IME::Vector2u &boundingRect) :
        Entity(boundingRect, IME::Entity::Type::Player),
        numberOfLives_(Constants::PacManLives),
        speed_(0.0f),
        isMoving_(false)
    {
        setCollidable(true);
        auto animations = PacManAnimations();
        animations.create();
        for (const auto& animation : animations.getAll())
            sprite_.addAnimation(animation);

        setDirection(IME::Direction::Left);
        sprite_.switchAnimation("goingLeft");
        sprite_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
        sprite_.scale(2.0f, 2.0f);
        sprite_.setPosition(getSize().x / 2.0f, getSize().y / 2.0f);

        onEvent("positionChanged", IME::Callback<float, float>([this](float x, float y) {
            sprite_.setPosition(x + getSize().x / 2.0f, y + getSize().y / 2.0f);
        }));

        onEvent("directionChanged", IME::Callback<IME::Direction>([this](IME::Direction dir) {
            switch (dir) {
                case IME::Direction::None:
                    break;
                case IME::Direction::Left:
                    sprite_.switchAnimation("goingLeft");
                    break;
                case IME::Direction::Right:
                    sprite_.switchAnimation("goingRight");
                    break;
                case IME::Direction::Up:
                    sprite_.switchAnimation("goingUp");
                    break;
                case IME::Direction::Down:
                    sprite_.switchAnimation("goingDown");
                    break;
            }
        }));
    }

    std::string PacMan::getClassType() {
        return "PacMan";
    }

    void PacMan::setNumberOfLives(unsigned int numOfLives) {
        if (numOfLives < 0 || (numberOfLives_ == 0 && !isVulnerable()))
            return;

        numberOfLives_ = numOfLives;
        if (numberOfLives_ == 0)
            setAlive(false);
    }

    unsigned int PacMan::getNumberOfLives() const {
        return numberOfLives_;
    }

    void PacMan::update(float deltaTime) {
        if (isMoving()) {
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

    void PacMan::pushState(std::shared_ptr<IState> state) {
        stateController_.pushState(std::move(state));
    }

    void PacMan::popState() {
        stateController_.popState();
    }

    IME::Graphics::AnimatableSprite &PacMan::getSprite() {
        return sprite_;
    }

    void PacMan::move() {
        if (speed_ > 0)
            isMoving_ = true;
    }

    void PacMan::setSpeed(float speed) {
        if (speed < 0.0f)
            speed_ = 0.0f;
        else
            speed_ = speed;
    }

    float PacMan::getSpeed() const {
        return speed_;
    }

    bool PacMan::isMoving() const {
        return isMoving_;
    }

    void PacMan::stop() {
        isMoving_ = false;
    }
}
