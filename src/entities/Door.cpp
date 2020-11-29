#include "Door.h"
#include "../common/SpriteContainer.h"

namespace SuperPacMan {
    Door::Door(const IME::Vector2u &boundingRect) :
        Entity(boundingRect, IME::Entity::Type::Obstacle),
        orientation_(Orientation::Vertical)
    {
        setCollidable(true);
        sprite_ = SpriteContainer::getSprite("unlocked_door");
        sprite_.setOrigin(sprite_.getSize() / 2.0f);
        sprite_.scale(2.07f, 2.07f);
        sprite_.setPosition(getSize().x / 2.0f, getSize().y / 2.0f);

        onEvent("positionChanged", IME::Callback<float, float>([this](float x, float y) {
            sprite_.setPosition(x + getSize().x / 2.0f, y + getSize().y / 2.0f);
        }));

        onEvent("locked", IME::Callback<>([this] {
            if (orientation_ == Orientation::Horizontal)
                sprite_.setTextureRect(SpriteContainer::getSprite("locked_door_horizontal").getTextureRect());
            else
                sprite_.setTextureRect(SpriteContainer::getSprite("locked_door_vertical").getTextureRect());
        }));

        onEvent("broken", IME::Callback<>([this] {
            if (orientation_ == Orientation::Horizontal)
                sprite_.setTextureRect(SpriteContainer::getSprite("broken_door_horizontal").getTextureRect());
            else
                sprite_.setTextureRect(SpriteContainer::getSprite("broken_door_vertical").getTextureRect());
        }));

        onEvent("unlocked", IME::Callback<>([this] {
            sprite_.setTextureRect(SpriteContainer::getSprite("unlocked_door").getTextureRect());
        }));
    }

    Door::Door(const Door &other)
        : IME::Entity(*this), doorLocker_(std::make_unique<DoorLocker>(*other.doorLocker_)),
          orientation_(other.orientation_)
    {}

    Door::Door(Door &&other) noexcept
        : IME::Entity(*this), doorLocker_(std::move(other.doorLocker_)),
          orientation_(other.orientation_)
    {}

    void Door::setOrientation(Orientation orientation) {
        orientation_ = orientation;
    }

    Door &Door::operator=(Door rhs) {
        std::swap(*this, rhs);
        return *this;
    }

    bool Door::isLocked() const {
        if (doorLocker_)
            return doorLocker_->isLocked();
        return false;
    }

    void Door::addDoorLocker(std::unique_ptr<DoorLocker> doorLocker) {
        doorLocker_ = std::move(doorLocker);
    }

    void Door::lockWith(const Key &key) {
        if (doorLocker_ && !isLocked()) {
            doorLocker_->lock(key);
            if (doorLocker_->isLocked()) {
                setCollidable(true);
                publishEvent("locked");
            }
        }
    }

    void Door::unlockWith(const Key &key) {
        if (doorLocker_ && isLocked()) {
            doorLocker_->unlock(key);
            if (!doorLocker_->isLocked()) {
                setCollidable(false);
                publishEvent("unlocked");
            }
        }
    }

    void Door::forceOpen() {
        if (doorLocker_) {
            doorLocker_ = nullptr;
            setCollidable(false);
            publishEvent("lockerBroken");
        }
    }

    int Door::onLock(IME::Callback<> callback) {
        return onEvent("locked", std::move(callback));
    }

    int Door::onUnlock(IME::Callback<> callback) {
        return onEvent("unlocked", std::move(callback));
    }

    Orientation Door::getOrientation() const {
        return orientation_;
    }

    std::string Door::getClassType() {
        return "Door";
    }

    IME::Graphics::Sprite &Door::getSprite() {
        return sprite_;
    }
}
