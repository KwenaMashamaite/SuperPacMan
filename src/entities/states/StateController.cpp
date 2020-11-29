#include "StateController.h"
#include <cassert>

namespace SuperPacMan {
    void StateController::pushState(std::shared_ptr<IState> state) {
        assert(state && "State cannot be null");
        state->onStart();
        states_.push(std::move(state));
    }

    std::shared_ptr<IState> StateController::getCurrentState() {
        return !states_.empty() ? states_.top() : nullptr;
    }

    void StateController::popState() {
        if (states_.empty())
            return;
        states_.top()->onExit();
        states_.pop();
    }

    bool StateController::isEmpty() const {
        return states_.empty();
    }
}
