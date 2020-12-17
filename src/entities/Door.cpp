////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "Door.h"
#include "../common/SpriteContainer.h"

namespace SuperPacMan {
    Door::Door(const IME::Vector2u &boundingRect) :
        Entity(boundingRect, IME::Entity::Type::Obstacle),
        orientation_(Orientation::Vertical)
    {
        setCollidable(true);
        sprite_ = SpriteContainer::getSprite("unlocked_door");
        sprite_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
        sprite_.scale(2.07f, 2.07f);

        onEvent("positionChange", IME::Callback<float, float>([this](float x, float y) {
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

    Door &Door::operator=(Door rhs) {
        std::swap(*this, rhs);
        return *this;
    }

    void Door::setOrientation(Orientation orientation) {
        orientation_ = orientation;
    }

    Orientation Door::getOrientation() const {
        return orientation_;
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

    bool Door::isLocked() const {
        if (doorLocker_)
            return doorLocker_->isLocked();
        return false;
    }

    void Door::forceOpen() {
        setCollidable(false);
        publishEvent("broken");
    }

    int Door::onLock(IME::Callback<> callback) {
        return onEvent("locked", std::move(callback));
    }

    int Door::onUnlock(IME::Callback<> callback) {
        return onEvent("unlocked", std::move(callback));
    }

    std::string Door::getClassType() {
        return "Door";
    }

    IME::Graphics::Sprite &Door::getSprite() {
        return sprite_;
    }
}
