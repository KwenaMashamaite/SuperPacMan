#include "Key.h"
#include "../common/SpriteContainer.h"

namespace SuperPacMan {
    Key::Key(const IME::Vector2u &boundingRect, int id) :
        Entity(boundingRect, IME::Entity::Type::Collectable),
        id_(id)
    {
        setCollidable(true);
        sprite_ = SpriteContainer::getSprite("key");
        sprite_.setOrigin(sprite_.getSize() / 2.0f);
        sprite_.scale(2.0f, 2.0f);
        sprite_.setPosition(getSize().x / 2.0f, getSize().y / 2.0f);

        onEvent("positionChanged", IME::Callback<float, float>([this](float x, float y) {
            sprite_.setPosition(x + getSize().x / 2.0f, y + getSize().y / 2.0f);
        }));
    }

    std::string Key::getClassType() {
        return "Key";
    }

    int Key::getId() const {
        return id_;
    }

    IME::Graphics::Sprite &Key::getSprite() {
        return sprite_;
    }
}
