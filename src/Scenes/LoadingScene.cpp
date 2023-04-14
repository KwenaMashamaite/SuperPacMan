
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

#include "LoadingScene.h"
#include "views/LoadingSceneView.h"
#include <Mighter2d/core/engine/Engine.h>
#include <Mighter2d/ui/widgets/ProgressBar.h>
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/core/resources/ResourceLoader.h>
#include <thread>

namespace spm {
    ///////////////////////////////////////////////////////////////
    void loadGameAssets() {
        mighter2d::ResourceLoader::loadFromFile(mighter2d::ResourceType::Font, {
            "ChaletLondonNineteenSixty.ttf", "AtariClassicExtrasmooth-LxZy.ttf",
            "namco.ttf", "pacfont.ttf", "DejaVuSans.ttf"});

        mighter2d::ResourceLoader::loadFromFile(mighter2d::ResourceType::Texture,  {
            "icon.png", "pacman_logo.png", "spritesheet.png",
            "main_menu_background.jpg", "main_menu_background_blurred.jpg"
        });

        mighter2d::ResourceLoader::loadFromFile(mighter2d::ResourceType::SoundEffect, {
            "doorBroken.wav", "fruitEaten.wav", "ghostEaten.wav",
            "pacmanDying.wav", "powerPelletEaten.wav", "superPelletEaten.wav",
            "beginning.wav", "levelComplete.ogg", "wieu_wieu_slow.ogg", "extraLife.wav",
            "starSpawned.wav", "bonusFruitMatch.wav", "bonusFruitNotMatch.wav", "ghostsTurnedBlue.wav"
        });

        mighter2d::ResourceLoader::loadFromFile(mighter2d::ResourceType::Music, {
            "searching.ogg", "pacman_intermission.ogg"
        });
    }

    ///////////////////////////////////////////////////////////////
    void LoadingScene::onEnter() {
        getWindow().setDefaultOnCloseHandlerEnable(false);
        LoadingSceneView::init(getGui());

        getGui().getWidget("pbrAssetLoading")->on("full", mighter2d::Callback<>([this] {
            getGui().getWidget<mighter2d::ui::Label>("lblLoading")->setText("Resources loaded successfully");
            getEngine().popScene();
        }));

        getTimer().setInterval(mighter2d::milliseconds(65), [this] {
            getGui().getWidget<mighter2d::ui::ProgressBar>("pbrAssetLoading")->incrementValue();
        });

        // The progress bar and the thread are not in sync. The thread finishes
        // almost intently and the progress bar is just for user experience
        std::thread(&loadGameAssets).detach();
    }

    ///////////////////////////////////////////////////////////////
    void LoadingScene::onExit() {
        getEngine().pushCachedScene("MainMenuScene");
    }

} // namespace pm
