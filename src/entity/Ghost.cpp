#include "Ghost.h"

namespace SuperPacMan {
    Ghost::Ghost(GhostName ghostName, const IME::Vector2u &boundingRect)
        : Entity(boundingRect), ghostName_(ghostName)
    {}

    void Ghost::setState(Ghost::State state) {
        if (state_ != state) {
            switch (state) {
                case State::Chase:
                    break;
                case State::Frightened:
                    break;
            }
        }
    }

    Ghost::State Ghost::getState() const {
        return Ghost::State::Frightened;
    }

    std::string Ghost::getType() {
        return "Ghost";
    }

    GhostName Ghost::getGhostName() const {
        return ghostName_;
    }
}

