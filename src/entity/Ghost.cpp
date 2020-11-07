#include "Ghost.h"

namespace SuperPacMan {
    Ghost::Ghost(GhostName ghostName, const IME::Dimensions &boundingRect)
        : Entity(boundingRect), ghostName_(ghostName)
    {}

    std::string Ghost::getType() {
        return "Ghost";
    }

    GhostName Ghost::getGhostName() const {
        return ghostName_;
    }
}

