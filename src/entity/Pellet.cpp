#include "Pellet.h"

namespace SuperPacMan {
    Pellet::Pellet(PelletType pelletType, const IME::Dimensions &boundingRect)
        : Entity(boundingRect), pelletType_(pelletType)
    {}

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

    std::string Pellet::getType() {
        return "Pellet";
    }
}
