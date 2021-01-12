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

#include "StartUpState.h"
#include <ime/core/loop/Engine.h>
#include <IME/ui/widgets/Panel.h>

namespace pacman {
    StartUpState::StartUpState(ime::Engine &engine) :
        State(engine),
        view_(engine.getRenderTarget()),
        isInit_{false},
        timeOut_{25.0f}
    {}

    void StartUpState::onEnter() {
        view_.init();

        //Make state skippable by pressing enter key
        engine().getInputManager().addKeyListener(ime::input::Keyboard::Event::KeyUp,
            ime::input::Keyboard::Key::Enter, [this] {
            engine().popState();
        });

        isInit_ = true;
    }

    bool StartUpState::isEntered() const {
        return isInit_;
    }

    void StartUpState::handleEvent(sf::Event event) {

    }

    void StartUpState::update(float deltaTime) {
        timeOut_ -= deltaTime;
        static auto animationBegan = false;
        if (timeOut_ <= 0.0f && !animationBegan) {
            animationBegan = true;
            auto viewContainer = view_.getWidget<ime::ui::Panel>("container");
            viewContainer->hideWithEffect(ime::ShowAnimationType::SlideFromBottom, 2000);
            viewContainer->on("animationFinish", ime::Callback<>([this] {
                engine().popState();
            }));
        }
    }

    void StartUpState::fixedUpdate(float deltaTime) {

    }

    void StartUpState::render(ime::Window &renderTarget) {
        view_.render();
    }

    void StartUpState::onPause() {

    }

    void StartUpState::onResume() {

    }

    void StartUpState::onExit() {

    }
}
