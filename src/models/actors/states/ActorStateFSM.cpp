////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "src/models/actors/states/ActorStateFSM.h"
#include <cassert>

namespace spm {
    ActorStateFSM::ActorStateFSM() :
        isStarted_{false}
    {}

    void ActorStateFSM::start() {
        if (!isStarted_) {
            isStarted_ = true;
            if (!states_.empty())
                states_.top()->onEntry();
        }
    }

    void ActorStateFSM::push(std::unique_ptr<IActorState> state) {
        assert(state && "Cannot push a nullptr to the FSM");

        if (!states_.empty())
            states_.top()->onPause();

        states_.push(std::move(state));

        if (isStarted_)
            states_.top()->onEntry();
    }

    void ActorStateFSM::popAndPush(std::unique_ptr<IActorState> state) {
        if (!states_.empty()) {
            states_.top()->onExit();
            states_.pop();
        }

        states_.push(std::move(state));
        states_.top()->onEntry();
    }

    void ActorStateFSM::pop() {
        if (states_.empty())
            return;

        std::unique_ptr poppedState = std::move(states_.top());
        states_.pop();
        poppedState->onExit();

        if (!states_.empty())
            states_.top()->onResume();
    }

    IActorState *ActorStateFSM::top() {
        return states_.empty() ? nullptr : states_.top().get();
    }

    std::size_t ActorStateFSM::getCount() const {
        return states_.size();
    }

    void ActorStateFSM::clear() {
        auto emptyStack = std::stack<std::unique_ptr<IActorState>>{};
        states_.swap(emptyStack);
    }
}
