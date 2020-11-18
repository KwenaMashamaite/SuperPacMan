#include "Key.h"

namespace SuperPacMan {
    Key::Key(const IME::Vector2u &boundingRect, int id) : Entity(boundingRect), id_(id)
    {}

    std::string Key::getType() {
        return "Key";
    }

    int Key::getId() const {
        return id_;
    }
}
