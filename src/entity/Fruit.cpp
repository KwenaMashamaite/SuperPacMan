#include "Fruit.h"

namespace SuperPacMan {
    Fruit::Fruit(const IME::Vector2u &boundingRect, const std::string &name)
        : Entity(boundingRect), name_(name), isEaten_(false)
    {}

    std::string SuperPacMan::Fruit::getType() {
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
}
