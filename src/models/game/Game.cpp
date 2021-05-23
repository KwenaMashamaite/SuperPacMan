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

#include "src/models/game/Game.h"
#include "src/models/scoreboard/Scoreboard.h"
#include "src/models/scenes/StartUpScene.h"
#include <IME/core/event/EventDispatcher.h>
#include <IME/ui/widgets/Panel.h>
#include <IME/ui/widgets/MessageBox.h>

namespace spm {
    Game::Game() :
        engine_{"Super Pac-Man", "assets/textFiles/settings.txt"}
    {}

    void Game::initialize() {
        engine_.initialize();

        //Data that should be accessible in all states
        auto scoreboard = Scoreboard("assets/textFiles/highscores.txt");
        scoreboard.load();

        engine_.getPersistentData().addProperty({"highScore", scoreboard.getTopScore().getValue()});
        engine_.getPersistentData().addProperty({"level", 1});
        engine_.getPersistentData().addProperty({"score", 0});
        engine_.getPersistentData().addProperty({"lives", 4});
        engine_.pushScene(std::make_unique<StartUpScene>());

        // This event is emitted by the LoadingScene after all assets have been loaded.
        // While the game is in the LoadingScene, it cannot be exited, so after loading
        // assets, re-enable the quit functionality
        ime::EventDispatcher::instance()->onEvent("assetLoadFinish", ime::Callback<>([this] {
            engine_.getInputManager().onKeyUp([this] (ime::Key key) {
                if (key == ime::Key::Escape)
                    openQuitPrompt();
            });

            engine_.onWindowClose([this] {
                openQuitPrompt();
            });
        }));
    }

    void Game::start() {
        engine_.run();
    }

    void Game::openQuitPrompt() {
        auto panel = ime::ui::Panel::create();
        panel->getRenderer()->setBackgroundColour(ime::Colour::Black);
        panel->getRenderer()->setOpacity(0.5);
        engine_.getGui().addWidget(std::move(panel), "pnl");

        auto* messageBox = engine_.getGui().addWidget<ime::ui::MessageBox>(ime::ui::MessageBox::create("Exit game"),"msgBoxQuitGame");
        messageBox->getRenderer()->setBorderBelowTitleBar(2.0f);
        messageBox->getRenderer()->setShowTextOnTitleButtons(true);
        messageBox->getRenderer()->setShowTextOnTitleButtons(true);
        messageBox->getRenderer()->setDistanceToSide(10.0f);
        messageBox->setText("Are you sure you want to quit ?");
        messageBox->addButton("Quit");
        messageBox->addButton("Cancel");
        messageBox->setPosition("50%", "50%");
        messageBox->setOrigin(0.5f, 0.5f);
        messageBox->getRenderer()->setFocusedBorderColour(ime::Colour::Blue);
        messageBox->getRenderer()->setBorders({1, 1, 1, 1});
        messageBox->setDraggable(false);

        messageBox->on("buttonPress", ime::Callback<std::string>([this, messageBox](const std::string& button) {
            if (button == "Quit")
                engine_.quit();
            else {
                messageBox->close();
                engine_.getGui().removeWidget("pnl");
                engine_.getGui().removeWidget("msgBoxQuitGame");
                engine_.setPause(false);
            }
        }));

        messageBox->on("escapeKeyPress", ime::Callback<>([this, messageBox] {
            messageBox->close();
            engine_.setPause(false);
            engine_.getGui().removeWidget("pnl");
            engine_.getGui().removeWidget("msgBoxQuitGame");
        }));

        engine_.setPause(true);
    }

    void Game::closeQuitPPromp() {

    }
}
