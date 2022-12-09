////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#include "PauseMenuScene.h"
#include "views/PauseMenuSceneView.h"
#include <IME/ui/widgets/ToggleButton.h>
#include <IME/core/engine/Engine.h>

namespace spm {
    ///////////////////////////////////////////////////////////////
    void PauseMenuScene::onEnter() {
        PauseMenuSceneView::init(getGui());
        initEventHandlers();
    }

    ///////////////////////////////////////////////////////////////
    void PauseMenuScene::initEventHandlers() {
        getGui().getWidget("btnResume")->on("click", ime::Callback<>([this] {
            getEngine().popScene();
        }));

        getGui().getWidget("btnMainMenu")->on("click", ime::Callback<>([this] {
            getEngine().getPreviousScene()->setCached(true, "GameplayScene");
            getEngine().popScene();
            getEngine().popScene();
            getEngine().pushCachedScene("MainMenuScene");
        }));

        getGui().getWidget("btnExit")->on("click", ime::Callback<>([this] {
            getEngine().quit();
        }));

        auto btnOption = getGui().getWidget<ime::ui::ToggleButton>("btnAudioToggle");
        btnOption->setChecked(getCache().getValue<float>("MASTER_VOLUME") > 0.0f);
        btnOption->setText(btnOption->isChecked() ? "on" : "off");

        getGui().getWidget("btnAudioToggle")->on("toggle", ime::Callback<bool>([this, btnOption](bool checked) {
            if (checked) {
                getCache().setValue("MASTER_VOLUME", 100.0f);
                btnOption->setText("on");
            } else {
                getCache().setValue("MASTER_VOLUME", 0.0f);
                btnOption->setText("off");
            }
        }));

        getInput().onKeyUp([this](ime::Keyboard::Key key) {
            if (key == ime::Keyboard::Key::Escape || key == ime::Keyboard::Key::P)
                getEngine().popScene();
        });
    }

} // namespace pm
