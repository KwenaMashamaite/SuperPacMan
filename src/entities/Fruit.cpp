#include "Fruit.h"
#include "../common/SpriteContainer.h"

namespace SuperPacMan {
    Fruit::Fruit(const IME::Vector2u &boundingRect, const std::string &name) :
        Entity(boundingRect, IME::Entity::Type::Collectable),
        name_(name),
        isEaten_(false)
    {
        setCollidable(true);
        sprite_ = SpriteContainer::getSprite(name);
        sprite_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
        sprite_.scale(2.0f, 2.0f);
        sprite_.setPosition(getSize().x / 2.0f, getSize().y / 2.0f);

        onEvent("positionChanged", IME::Callback<float, float>([this](float x, float y) {
            sprite_.setPosition(x + getSize().x / 2.0f, y + getSize().y / 2.0f);
        }));
    }

    std::string SuperPacMan::Fruit::getClassType() {
        return "Fruit";
    }

    const std::string &Fruit::getName() const {
        return name_;
    }

    void Fruit::eat() {
        if (!isEaten_ && isVulnerable()){
            isEaten_ = true;
            setAlive(false);
            setCollidable(false);
            publishEvent("eaten");
        }
    }

    bool Fruit::isEaten() const {
        return isEaten_;
    }

    IME::Graphics::Sprite &Fruit::getSprite() {
        return sprite_;
    }
}
