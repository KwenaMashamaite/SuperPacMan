#include "PacMan.h"

namespace SuperPacMan {
    PacMan::PacMan(const IME::Dimensions &boundingRect) : Entity(boundingRect), numberOfLives_(1)
    {}

    std::string PacMan::getType() {
        return "PacMan";
    }

    void PacMan::setNumberOfLives(unsigned int numOfLives) {
        if (numOfLives < 0 || (numberOfLives_ == 0 && !isVulnerable()))
            return;

        numberOfLives_ = numOfLives;
        if (numberOfLives_ == 0)
            setAlive(false);
    }

    unsigned int PacMan::getNumberOfLives() const {
        return numberOfLives_;
    }
}
