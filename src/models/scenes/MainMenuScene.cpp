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

#include "src/models/scenes/MainMenuScene.h"
#include "src/models/scenes/GameplayScene.h"
#include "src/models/scenes/LevelStartScene.h"
#include "src/models/scoreboard/Scoreboard.h"
#include <IME/ui/widgets/VerticalLayout.h>
#include <IME/core/engine/Engine.h>
#include <IME/ui/widgets/Label.h>
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    MainMenuScene::MainMenuScene() :
        view_{gui()}
    {}

    ///////////////////////////////////////////////////////////////
    void MainMenuScene::onEnter() {
        engine().getWindow().onClose(nullptr); // Let window be closed with exit button only

        view_.init();

        auto scoreboard = cache().getValue<std::shared_ptr<Scoreboard>>("SCOREBOARD");

        const int NUM_SCORES_TO_DISPLAY = 10;
        assert(scoreboard->getSize() >= NUM_SCORES_TO_DISPLAY && "Scoreboard must have at least 10 entries");

        auto namesContainer = gui().getWidget<ime::ui::VerticalLayout>("vlNames");
        auto scoreContainer = gui().getWidget<ime::ui::VerticalLayout>("vlScores");
        auto levelContainer = gui().getWidget<ime::ui::VerticalLayout>("vlLevels");

        // Replace placeholder text with actual scoreboard data
        scoreboard->forEachScore([&, count = 1] (const Score& score) mutable {
            if (count > NUM_SCORES_TO_DISPLAY)
                return;

            namesContainer->getWidget<ime::ui::Label>("lblEntry" + std::to_string(count))->setText(score.getOwner());
            scoreContainer->getWidget<ime::ui::Label>("lblEntry" + std::to_string(count))->setText(std::to_string(score.getValue()));
            levelContainer->getWidget<ime::ui::Label>("lblEntry" + std::to_string(count))->setText(std::to_string(score.getLevel()));
            count++;
        });

        // Init event handlers
        gui().getWidget("btnPlay")->on("click", ime::Callback<>([this] {
            engine().popScene();
            engine().pushScene(std::make_unique<GameplayScene>());
            engine().pushScene(std::make_unique<LevelStartScene>());
        }));

        gui().getWidget("btnQuit")->on("click", ime::Callback<>([this] {
            engine().quit();
        }));
    }

} // namespace spm
