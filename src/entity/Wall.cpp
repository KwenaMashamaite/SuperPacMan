#include "Wall.h"

namespace SuperPacMan {
    Wall::Wall(const IME::Vector2u &boundingRect) : Entity(boundingRect)
    {
        setCollidable(true);
    }

    std::string Wall::getType() {
        return "Wall";
    }
}
