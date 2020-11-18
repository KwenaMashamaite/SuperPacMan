#include "PacMan.h"
#include "../common/Constants.h"

namespace SuperPacMan {
    PacMan::PacMan(const IME::Vector2u &boundingRect) : Entity(boundingRect),
        numberOfLives_(1), state_(State::Normal)
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

    void PacMan::switchState(PacMan::State state) {
        if (state_ != state) {
            switch (state) {
                case State::Normal:
                    setSpeed(Constants::PacManNormalSpeed);
                    setVulnerable(true);
                    break;
                case State::Super:
                    setSpeed(Constants::PacManSuperSizeSpeed);
                    setVulnerable(false);
                    break;
            }
            publishEvent("stateChange", state_);
        }
    }

    PacMan::State PacMan::getState() const {
        return state_;
    }
}
