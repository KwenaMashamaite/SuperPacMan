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

#include "src/models/scenes/GameOverScene.h"
#include "src/models/scoreboard/Scoreboard.h"
#include "src/models/scenes/LevelStartScene.h"
#include "src/models/scenes/GameplayScene.h"
#include "src/models/scenes/MainMenuScene.h"
#include "src/common/Constants.h"
#include <IME/core/engine/Engine.h>
#include <IME/ui/widgets/EditBox.h>
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/Button.h>
#include <IME/utility/Utils.h>

using namespace ime::ui;

namespace spm {
    void GameOverScene::onEnter() {
        engine().onWindowClose(nullptr);

        view_.init(gui(), cache().getValue<bool>("playerWon"));

        // Replace placeholder text with actual text
        gui().getWidget<Label>("lblHighScoreVal")->setText(std::to_string(cache().getValue<int>("highScore")));
        gui().getWidget<Label>("lblScoreVal")->setText(std::to_string(cache().getValue<int>("score")));

        initEvents();
    }

    void GameOverScene::initEvents() {
        // Update scoreboard file on disk when player clicks "save" button
        gui().getWidget<Button>("btnSave")->on("click", ime::Callback<>([this] {
            auto name = gui().getWidget<EditBox>("txtName")->getText();

            if (name.empty()) { // Create random name
                const auto MAX_NUM_CHARACTERS = 10;
                for (auto i = 0; i < MAX_NUM_CHARACTERS; i++) {
                    name += static_cast<char>(ime::utility::generateRandomNum(97, 122));
                }

                // Capitalize first letter
                name[0] = static_cast<char>(toupper(name[0]));
            }

            // Add player score to leaderboard
            auto playerScore = cache().getValue<int>("score");
            auto playerLevel = cache().getValue<int>("level");

            auto score = Score();
            score.setValue(playerScore);
            score.setLevel(playerLevel);
            score.setOwner(name);

            auto scoreboard = cache().getValue<std::shared_ptr<Scoreboard>>("scoreboard");
            scoreboard->addScore(score);
            scoreboard->updateHighScoreFile();
        }));

        // If the player completes the game, the retry button is not available
        if (!cache().getValue<bool>("playerWon")) {
            // Replenish pacmans lives and restart level when "Restart Level" button is clicked
            gui().getWidget("btnRetryLevel")->on("click", ime::Callback<>([this] {
                cache().setValue("lives", Constants::PacManLives);
                engine().popScene();
                engine().pushScene(std::make_unique<GameplayScene>());
                engine().pushScene(std::make_unique<LevelStartScene>());
            }));
        }

        // Exit to the games main menu when "Exit to Main Menu" is clicked
        gui().getWidget("btnExitMainMenu")->on("click", ime::Callback<>([this] {
            cache().setValue("level", 1);
            cache().setValue("score", 0);
            cache().setValue("lives", Constants::PacManLives);

            engine().popScene();
            engine().pushScene(std::make_unique<MainMenuScene>());
        }));

        // Exit to desktop when "Exit Game" button is clicked
        gui().getWidget("btnExitGame")->on("click", ime::Callback<>([this] {
            engine().quit();
        }));
    }
}
