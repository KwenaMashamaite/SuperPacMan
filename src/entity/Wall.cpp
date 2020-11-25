#include "Wall.h"

namespace SuperPacMan {
    Wall::Wall(const IME::Vector2u &boundingRect) :
        Entity(boundingRect, IME::Entity::Type::Obstacle)
    {
        setCollidable(true);
    }

    std::string Wall::getClassType() {
        return "Wall";
    }
}
