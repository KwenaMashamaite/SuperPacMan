#include "Movable.h"

namespace SuperPacMan {
    Movable::Movable() : Movable(0.0f) {}

    Movable::Movable(float speed) : speed_(speed), isMoving_(false)
    {}

    void Movable::move() {
        isMoving_ = true;
    }

    void Movable::setSpeed(float speed) {
        speed_ = speed;
        if (speed_ <= 0)
            stop();
    }

    float Movable::getSpeed() const {
        return speed_;
    }

    bool Movable::isMoving() const {
        return isMoving_;
    }

    void Movable::stop() {
        isMoving_ = false;
    }
}