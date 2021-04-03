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

#include "src/models/scenes/StartUpScene.h"
#include "src/models/scenes/LoadingScene.h"
#include "src/views/StartUpSceneView.h"
#include <IME/core/engine/Engine.h>
#include <IME/ui/widgets/Panel.h>

namespace spm {
    void StartUpScene::onEnter() {
        StartUpSceneView::init(gui());

        auto pnlContainer = gui().getWidget<ime::ui::Panel>("pnlContainer");
        pnlContainer->showWithEffect(ime::ShowAnimationType::Fade, ime::seconds(3));

        // Exit the scene with an effect after a couple of seconds
        timer().setTimeout(ime::seconds(13), [this, pnlContainer] {
            pnlContainer->hideWithEffect(ime::ShowAnimationType::SlideFromBottom, ime::seconds(2));
            pnlContainer->on("animationFinish", ime::Callback<>([this] {
                engine().popScene();
            }));
        });

        // Make the scene skippable by pressing the enter/return key
        input().onKeyUp([this, pnlContainer](ime::Key key) {
            if (key == ime::Key::Enter)
                engine().popScene();
        });
    }

    void StartUpScene::onExit() {
        gui().removeAllWidgets();
        engine().pushScene(std::make_unique<LoadingScene>());
    }
}
