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

#include "StartUpScene.h"
//#include "LoadingScene.h"
#include "Views/StartUpSceneView.h"
#include <Mighter2d/core/engine/Engine.h>
#include <Mighter2d/ui/widgets/Panel.h>
#include <Mighter2d/ui/widgets/EditBox.h>
#include <Mighter2d/ui/widgets/Button.h>

using namespace mighter2d::ui;

namespace spm {
    ///////////////////////////////////////////////////////////////
    StartUpScene::StartUpScene() :
        gui_(*this),
        keyboard_(*this),
        timer_(*this)
    {}

    ///////////////////////////////////////////////////////////////
    void StartUpScene::onStart() {
        StartUpSceneView().init(gui_);

        if (getCache().hasProperty("PLAYER_NAME")) {
            startCountdown();
            enableSceneSkip();
        } else
            promptName();
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::startCountdown() {
        auto pnlContainer = gui_.getWidget<Panel>("pnlContainer");
        pnlContainer->showWithEffect(AnimationType::Fade, mighter2d::seconds(3));

        // Exit the scene with an effect after a couple of seconds
        timer_.onTimeout([this, pnlContainer] {
            pnlContainer->hideWithEffect(AnimationType::Fade, mighter2d::seconds(2));
            pnlContainer->onAnimationFinish([this] { getEngine().popScene(); });
        });

        timer_.setInterval(mighter2d::seconds(13));
        timer_.start();
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::promptName() {
        gui_.getWidget<Panel>("pnlContainer")->setVisible(false);
        gui_.getWidget<Panel>("pnlNamePrompt")->setVisible(true);

        // Disable continue button when player input is less than required and enable when input size is valid
        gui_.getWidget<EditBox>("txtName")->onTextChange([this](const std::string& text) {
            auto* btnContinue = gui_.getWidget<Button>("btnContinue");
            if (!text.empty())
                btnContinue->setEnabled(true);
            else
                btnContinue->setEnabled(false);
        });

        // Save player name and continue to gameplay
        gui_.getWidget<EditBox>("txtName")->onEnterKeyPress([this](const std::string& text) {
            if (!text.empty()) {
                gui_.setMouseCursor(mighter2d::CursorType::Arrow);
                save();
            }
        });

        // Save player name and continue to gameplay
        gui_.getWidget<Button>("btnContinue")->onClick([this] {
            save();
        });
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::enableSceneSkip() {
        keyboard_.onKeyUp([this](mighter2d::Keyboard::Key key) {
            if (key == mighter2d::Keyboard::Key::Space)
                getEngine().popScene();
        });
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::save() {
        auto name = gui_.getWidget<EditBox>("txtName")->getText();

        // Save name to engine cache (So that other Scenes can get access to it)
        getCache().addProperty(mighter2d::Property{"PLAYER_NAME", name});

        // Save player name to the disk so that it can be accessed on next game run
        mighter2d::Preference preference("PLAYER_NAME", mighter2d::PrefType::String);
        preference.setValue(name);
        preference.setDescription("The name of the player");
        mighter2d::savePref(preference, getCache().getValue<std::string>("SETTINGS_FILENAME"));

        // Display game disclaimer and initiate scene pop countdown
        gui_.getWidget("pnlContainer")->setVisible(true);
        gui_.getWidget("pnlNamePrompt")->setVisible(false);

        startCountdown();
        enableSceneSkip();
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::onDestroy() {
        //getEngine().pushScene(std::make_unique<LoadingScene>());
    }

} // namespace pm
