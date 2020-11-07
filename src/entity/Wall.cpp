#include "Wall.h"

namespace SuperPacMan {
    Wall::Wall(const IME::Dimensions &boundingRect) : Entity(boundingRect)
    {
        setCollidable(true);
    }

    std::string Wall::getType() {
        return "Wall";
    }
}
