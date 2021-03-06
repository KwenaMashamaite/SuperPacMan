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

#include "src/models/scenes/LoadingScene.h"
#include "src/models/scenes/MainMenuScene.h"
#include "src/views/LoadingSceneView.h"
#include <IME/core/engine/Engine.h>
#include <IME/ui/widgets/ProgressBar.h>
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/Panel.h>
#include <IME/core/resources/ResourceLoader.h>
#include <thread>
#include <chrono>

namespace spm {
    namespace {
        //Warning!! This number must be updated each time a new resource is added to the
        // Number of assets to be loaded
        const auto numOfResources = 21;
    }

    ///////////////////////////////////////////////////////////////
    LoadingScene::LoadingScene() :
        loadingFinished_{false}
    {}

    ///////////////////////////////////////////////////////////////
    void LoadingScene::onEnter() {
        // Prevent the game from being exited while assets are being loaded
        // The resource loading thread must finish first before we can stop
        // the main thread
        engine().getWindow().onClose(nullptr);

        LoadingSceneView::init(gui());
        auto pbrAssetLoading = gui().getWidget<ime::ui::ProgressBar>("pbrAssetLoading");
        pbrAssetLoading->setMaximumValue(numOfResources);

        // Update progress bar text and loading text to indicate assets loaded successfully
        pbrAssetLoading->on("full", ime::Callback<>([this, pbrAssetLoading] {
            gui().getWidget<ime::ui::Label>("lblLoading")
                ->setText("Resources loaded successfully");

            pbrAssetLoading->setText("100%");
        }));

        // Transition to next scene after all assets are loaded
        engine().onFrameEnd([this] {
            if (loadingFinished_) {
                engine().popScene();
                engine().onFrameEnd(nullptr);
            }
        });

        auto pnlContainer = gui().getWidget<ime::ui::Panel>("pnlContainer");
        pnlContainer->showWithEffect(ime::ShowAnimationType::Fade, ime::seconds(0.5));

        // Initiate asset loading thread after view animation finishes
        pnlContainer->on("animationFinish", ime::Callback<>([this] {
            std::thread([this] {
                loadGameAssets();
            }).detach();
        }));
    }

    ///////////////////////////////////////////////////////////////
    void LoadingScene::loadGameAssets() {
        auto loadFromFile = [this](ime::ResourceType rType, std::initializer_list<std::string> assets) {
            auto lblLoading = gui().getWidget<ime::ui::Label>("lblLoading");
            switch (rType) {
                case ime::ResourceType::Texture:
                    lblLoading->setText("Loading textures...");
                    break;
                case ime::ResourceType::Font:
                    lblLoading->setText("Loading fonts...");
                    break;
                case ime::ResourceType::Image:
                    lblLoading->setText("Loading textures...");
                    break;
                case ime::ResourceType::SoundBuffer:
                    lblLoading->setText("Loading sound effects...");
                    break;
                case ime::ResourceType::Music:
                    lblLoading->setText("Loading music...");
                    break;
            }

            ime::ResourceLoader::loadFromFile(rType, assets, [this](const std::string& text) {
                auto pbrAssetLoading = gui().getWidget<ime::ui::ProgressBar>("pbrAssetLoading");

                // Resources load very fast (less than a second), so we simulate a delay between each load
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
                pbrAssetLoading->setText("Loading " + text + "...");
                pbrAssetLoading->incrementValue();
            });
        };

        loadFromFile(ime::ResourceType::Font, {
            "ChaletLondonNineteenSixty.ttf", "AtariClassicExtrasmooth-LxZy.ttf",
            "namco.ttf", "pacfont.ttf", "DejaVuSans.ttf"});

        loadFromFile(ime::ResourceType::Texture,  {
            "icon.png", "pacman_logo.png", "spritesheet.png",
            "main_menu_background.jpg", "credits_menu_background.jpg"
        });

        loadFromFile(ime::ResourceType::SoundBuffer, {
            "doorBroken.wav", "fruitEaten.wav", "ghostEaten.wav",
            "pacmanDying.wav", "powerPelletEaten.wav", "superPelletEaten.wav",
            "beginning.wav", "levelComplete.ogg", "wieu_wieu_slow.ogg"
        });

        loadFromFile(ime::ResourceType::Music, {
            "searching.ogg", "pacman_intermission.ogg"
        });

        // Delay so that we can see the progress bar completing
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        loadingFinished_ = true;
    }

    ///////////////////////////////////////////////////////////////
    void LoadingScene::onExit() {
        engine().pushScene(std::make_unique<MainMenuScene>());
    }

} // namespace spm
