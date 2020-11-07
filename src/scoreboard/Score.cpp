#include "Score.h"

namespace SuperPacMan {
    Score::Score() : value_(0)
    {}

    bool Score::operator==(const Score &rhs) const {
        return value_ == rhs.value_ && owner_ == rhs.owner_;
    }

    bool Score::operator!=(const Score &rhs) const{
        return !(*this == rhs);
    }

    bool Score::operator<(const Score &rhs) const {
        return value_ < rhs.value_;
    }

    bool Score::operator>(const Score &rhs) const {
        return value_ > rhs.value_;
    }

    void Score::setValue(int value) {
        value_ = value;
    }

    void Score::adjustValue(int offset) {
        setValue(getValue() + offset);
    }

    void Score::setOwner(const std::string &name) {
        owner_ = name;
    }

    int Score::getValue() const {
        return value_;
    }

    const std::string &Score::getOwner() const {
        return owner_;
    }
}
