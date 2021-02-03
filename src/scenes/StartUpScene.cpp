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

#include "StartUpScene.h"
#include <ime/core/loop/Engine.h>
#include <IME/ui/widgets/Panel.h>

namespace pacman {
    StartUpScene::StartUpScene(ime::Engine &engine) :
        Scene(engine),
        view_(engine.getRenderTarget())
    {}

    void StartUpScene::onEnter() {
        view_.init();

        //Exit the scene with an effect after a certain time has passed
        timer().setTimeout(ime::seconds(15), [this] {
            auto viewContainer = view_.getWidget<ime::ui::Panel>("container");
            viewContainer->hideWithEffect(ime::ShowAnimationType::SlideFromBottom, 2000);
            viewContainer->on("animationFinish", ime::Callback<>([this] {
                engine().popScene();
            }));
        });

        //Make scene skippable by pressing enter key
        input().onKeyUp([this](ime::input::Keyboard::Key key) {
            if (key == ime::input::Keyboard::Key::Enter)
                engine().popScene();
        });
    }

    void StartUpScene::render(ime::Window &renderTarget) {
        view_.render();
    }

    void StartUpScene::handleEvent(ime::Event event) {}

    void StartUpScene::update(ime::Time deltaTime) {}
}
