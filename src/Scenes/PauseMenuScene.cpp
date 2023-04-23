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
#include <Mighter2d/ui/widgets/ToggleButton.h>
#include <Mighter2d/ui/widgets/Button.h>
#include <Mighter2d/core/engine/Engine.h>

namespace spm {
    using namespace mighter2d::ui;

    ///////////////////////////////////////////////////////////////
    PauseMenuScene::PauseMenuScene() :
        gui_(*this),
        keyboard_(*this)
    {}

    ///////////////////////////////////////////////////////////////
    void PauseMenuScene::onStart() {
        PauseMenuSceneView::init(gui_);
        initEventHandlers();
    }

    ///////////////////////////////////////////////////////////////
    void PauseMenuScene::initEventHandlers() {
        gui_.getWidget<Button>("btnResume")->onClick([this] {
            getEngine().popScene();
        });

        gui_.getWidget<Button>("btnMainMenu")->onClick([this] {
            getEngine().getPreviousScene()->setCacheOnExit(true, "GameplayScene");
            getEngine().popScene(2);
            getEngine().pushCachedScene("MainMenuScene");
        });

        gui_.getWidget<Button>("btnExit")->onClick([this] {
            getEngine().quit();
        });

        auto btnOption = gui_.getWidget<ToggleButton>("btnAudioToggle");
        btnOption->setChecked(getCache().getValue<float>("MASTER_VOLUME") > 0.0f);
        btnOption->setText(btnOption->isChecked() ? "on" : "off");

        gui_.getWidget<ToggleButton>("btnAudioToggle")->onToggle([this, btnOption](bool checked) {
            if (checked) {
                getCache().setValue("MASTER_VOLUME", 100.0f);
                btnOption->setText("on");
            } else {
                getCache().setValue("MASTER_VOLUME", 0.0f);
                btnOption->setText("off");
            }
        });

        keyboard_.onKeyUp([this](mighter2d::Keyboard::Key key) {
            if (key == mighter2d::Keyboard::Key::Escape || key == mighter2d::Keyboard::Key::P)
                getEngine().popScene();
        });
    }

} // namespace pm
