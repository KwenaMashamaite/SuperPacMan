#include "Pellet.h"
#include "../animations/PelletAnimations.h"

namespace SuperPacMan {
    Pellet::Pellet(PelletType pelletType, const IME::Vector2u &boundingRect) :
        Entity(boundingRect, IME::Entity::Type::Collectable),
        pelletType_(pelletType)
    {
        auto animations = PelletAnimations();
        animations.createAnimationFor(pelletType);
        for (const auto& animation : animations.getAll())
            sprite_.addAnimation(animation);

        sprite_.switchAnimation("blink");
        sprite_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
        sprite_.scale(2.0f, 2.0f);
        sprite_.setPosition(getPosition().x + getSize().x / 2.0f,getPosition().y + getSize().y / 2.0f);

        onEvent("positionChanged", IME::Callback<float, float>([this](float x, float y) {
            sprite_.setPosition(x + getSize().x / 2.0f, y + getSize().y / 2.0f);
        }));

        setCollidable(true);
    }

    void Pellet::eat() {
        if (!isEaten()){
            setAlive(false);
            setCollidable(false);
            publishEvent("eaten");
        }
    }

    bool Pellet::isEaten() const {
        return !isAlive();
    }

    PelletType Pellet::getPelletType() const {
        return pelletType_;
    }

    std::string Pellet::getClassType() {
        return "Pellet";
    }

    IME::Graphics::AnimatableSprite &Pellet::getSprite() {
        return sprite_;
    }

    void Pellet::update(float deltaTime) {
        sprite_.updateAnimation(deltaTime);
    }
}
