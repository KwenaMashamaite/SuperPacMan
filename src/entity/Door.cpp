#include "Door.h"

namespace SuperPacMan {
    Door::Door(const IME::Dimensions &boundingRect)
        : Entity(boundingRect), orientation_(Orientation::Vertical)
    {}

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

    std::string Door::getType() {
        return "Door";
    }
}
