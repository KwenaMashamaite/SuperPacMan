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

#include "src/models/scenes/GamePauseScene.h"
#include "src/views/GamePauseSceneView.h"
#include "src/models/scenes/MainMenuScene.h"
#include "src/models/scenes/GameplayScene.h"
#include "src/models/scenes/LevelStartScene.h"
#include <IME/ui/widgets/ToggleButton.h>
#include <IME/core/engine/Engine.h>

namespace spm {
    void GamePauseScene::onEnter() {
        //------------ Init pause menu gui and click event handlers ------------//
        GamePauseSceneView::init(gui());

        // 1. Resume button click handler
        gui().getWidget("btnResume")->on("click", ime::Callback<>([this] {
            engine().popScene();
        }));

        // 2. Restart button click handler
        gui().getWidget("btnRestart")->on("click", ime::Callback<>([this] {
            engine().removeAllScenesExceptActive(); // Popping scene twice will call GamePlayScene::onResume(), which we don't want to do
            engine().popScene();
            engine().pushScene(std::make_unique<GameplayScene>());
            engine().pushScene(std::make_unique<LevelStartScene>());
        }));

        // 3. Main menu button click handler
        gui().getWidget("btnMainMenu")->on("click", ime::Callback<>([this] {
            engine().removeAllScenesExceptActive();
            engine().popScene();
            engine().pushScene(std::make_unique<MainMenuScene>());
        }));

        // 4. Exit button click handler
        gui().getWidget("btnExit")->on("click", ime::Callback<>([this] {
            engine().quit();
        }));

        // 5. Audio toggle button click handler
        auto btnOption = gui().getWidget<ime::ui::ToggleButton>("btnAudioToggle");
        btnOption->setChecked(cache().getValue<float>("MASTER_VOLUME") > 0.0f);
        btnOption->setText(btnOption->isChecked() ? "on" : "off");

        gui().getWidget("btnAudioToggle")->on("toggle", ime::Callback<bool>([this, btnOption](bool checked) {
            if (checked) {
                cache().setValue("MASTER_VOLUME", 100.0f);
                btnOption->setText("on");
            } else {
                cache().setValue("MASTER_VOLUME", 0.0f);
                btnOption->setText("off");
            }
        }));

        // Return to game when escape is pressed
        input().onKeyUp([this](ime::Key key) {
           if (key == ime::Key::Escape)
               engine().popScene();
        });
    }
}
