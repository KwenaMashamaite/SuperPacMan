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
#include "LoadingScene.h"
#include "Views/StartUpSceneView.h"
#include <IME/core/engine/Engine.h>
#include <IME/ui/widgets/Panel.h>
#include <IME/ui/widgets/EditBox.h>
#include <IME/ui/widgets/Button.h>

using namespace ime::ui;

namespace spm {
    ///////////////////////////////////////////////////////////////
    void StartUpScene::onEnter() {
        StartUpSceneView().init(getGui());

        if (getCache().hasProperty("PLAYER_NAME")) {
            startCountdown();
            enableSceneSkip();
        } else
            promptName();
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::startCountdown() {
        auto pnlContainer = getGui().getWidget<ime::ui::Panel>("pnlContainer");
        pnlContainer->showWithEffect(ime::ui::AnimationType::Fade, ime::seconds(3));

        // Exit the scene with an effect after a couple of seconds
        getTimer().setTimeout(ime::seconds(13), [this, pnlContainer] {
            pnlContainer->hideWithEffect(ime::ui::AnimationType::Fade, ime::seconds(2));
            pnlContainer->on("animationFinish", ime::Callback<>([this] {
                getEngine().popScene();
            }));
        });
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::promptName() {
        getGui().getWidget<Panel>("pnlContainer")->setVisible(false);
        getGui().getWidget<Panel>("pnlNamePrompt")->setVisible(true);

        // Disable continue button when player input is less than required and enable when input size is valid
        getGui().getWidget("txtName")->on("textChange", ime::Callback<std::string>([this](const std::string& text) {
            auto* btnContinue = getGui().getWidget<Button>("btnContinue");
            if (!text.empty())
                btnContinue->setEnabled(true);
            else
                btnContinue->setEnabled(false);
        }));

        // Save player name and continue to gameplay
        getGui().getWidget("txtName")->on("enterKeyPress", ime::Callback<std::string>([this](const std::string& text) {
            if (!text.empty()) {
                getGui().setMouseCursor(ime::CursorType::Arrow);
                save();
            }
        }));

        // Save player name and continue to gameplay
        getGui().getWidget("btnContinue")->on("click", ime::Callback<>([this] {
            save();
        }));
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::enableSceneSkip() {
        getInput().onKeyUp([this](ime::Keyboard::Key key) {
            if (key == ime::Keyboard::Key::Space)
                getEngine().popScene();
        });
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::save() {
        auto name = getGui().getWidget<EditBox>("txtName")->getText();

        // Save name to engine cache (So that other Scenes can get access to it)
        getCache().addProperty(ime::Property{"PLAYER_NAME", name});

        // Save player name to the disk so that it can be accessed on next game run
        ime::Preference preference("PLAYER_NAME", ime::PrefType::String);
        preference.setValue(name);
        preference.setDescription("The name of the player");
        ime::savePref(preference, getCache().getValue<std::string>("SETTINGS_FILENAME"));

        // Display game disclaimer and initiate scene pop countdown
        getGui().getWidget<Panel>("pnlContainer")->setVisible(true);
        getGui().getWidget<Panel>("pnlNamePrompt")->setVisible(false);
        startCountdown();
        enableSceneSkip();
    }

    ///////////////////////////////////////////////////////////////
    void StartUpScene::onExit() {
        getEngine().pushScene(std::make_unique<LoadingScene>());
    }

} // namespace pm
