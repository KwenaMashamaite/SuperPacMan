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

#include "LoadingState.h"
#include <IME/core/loop/Engine.h>
#include <IME/ui/widgets/ProgressBar.h>
#include <IME/ui/widgets/Label.h>
#include <thread>
#include <chrono>
#include <IME/ui/widgets/Panel.h>
#include <IME/ui/widgets/VerticalLayout.h>

//Warning!! This number must be updated each time a new resource is added to the
//resources to be loaded
const auto numOfResources = 18;

namespace pacman {
    LoadingState::LoadingState(ime::Engine &engine) :
        State(engine),
        isEntered_{false},
        view_(engine.getRenderTarget()),
        loadingFinished_{false}
    {}

    void LoadingState::onEnter() {
        // Prevent the game from being exited while assets are being loaded
        // The resource loading thread must finish first before we can stop
        // the main thread
        engine().onWindowClose(nullptr);

        view_.init();
        auto progressBar = view_.getWidget<ime::ui::ProgressBar>("loading_progress_bar");
        progressBar->setMaximumValue(numOfResources);

        progressBar->on("full", ime::Callback<>([this, progressBar] {
            mtx_.unlock();
            view_.getWidget<ime::ui::Label>("loading_text")
                  ->setText("Resources loaded successfully");

            progressBar->setText("100%");
        }));

        // Transition to next state if all resources are loaded
        engine().onFrameEnd([this] {
            if (loadingFinished_) {
                engine().popState();
                engine().onFrameEnd(nullptr);
            }
        });

        //Initiate resource loading
        view_.getWidget<ime::ui::Panel>("container")->on("animationFinish", ime::Callback<>([this] {
            std::thread([this] {
                loadResources();
            }).detach();
        }));

        isEntered_ = true;
    }

    bool LoadingState::isEntered() const {
        return isEntered_;
    }

    void LoadingState::loadResources() {
        auto updateProgressBar = [this](const std::string& text) {
            static auto loadingProgressBar = view_.getWidget<ime::ui::ProgressBar>("loading_progress_bar");
            
            //Resources load very fast (less than a second), so we simulate a delay (= numberOfResources * threadSleepTime)
            std::this_thread::sleep_for(std::chrono::milliseconds (200));
            std::lock_guard<std::mutex> lock(mtx_);
            loadingProgressBar->setText("Loading " + text + "...");
            loadingProgressBar->incrementValue();
        };
        
        auto loadingText = view_.getWidget<ime::ui::Label>("loading_text");
        
        //LOAD FONTS
        std::unique_lock<std::mutex> lock(mtx_);
        loadingText->setText("Loading fonts...");
        lock.unlock();
        ime::ResourceManager::getInstance()->loadFromFile(ime::ResourceType::Font, {
            "namco.ttf", "AtariClassicExtrasmooth-LxZy.ttf"}, updateProgressBar);

        //LOAD TEXTURES
        lock.lock();
        loadingText->setText("Loading textures...");
        lock.unlock();
        ime::ResourceManager::getInstance()->loadFromFile(ime::ResourceType::Texture,  {
            "icon.png", "grids.png", "pacman_logo.png", "spritesheet.png",
            "main_menu_background.jpg", "credits_menu_background.jpg",
            "options_menu_background.jpg"
        }, updateProgressBar);

        //LOAD SOUND EFFECTS
        lock.lock();
        loadingText->setText("Loading sound effects...");
        lock.unlock();
        ime::ResourceManager::getInstance()->loadFromFile(ime::ResourceType::SoundBuffer, {
            "doorBroken.wav", "fruitEaten.wav", "ghostEaten.wav", "pacmanDying.wav",
            "powerPelletEaten.wav", "superPelletEaten.wav", "beginning.wav"
        }, updateProgressBar);

        //LOAD MUSIC
        lock.lock();
        loadingText->setText("Loading music...");
        lock.unlock();
        ime::ResourceManager::getInstance()->loadFromFile(ime::ResourceType::Music, {
            "searching.ogg", "pacman_intermission.ogg"
        }, updateProgressBar);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        loadingFinished_ = true;
    }

    void LoadingState::update(float deltaTime) {

    }

    void LoadingState::fixedUpdate(float deltaTime) {

    }

    void LoadingState::render(ime::Window &renderTarget) {
        std::lock_guard<std::mutex> lock(mtx_);
        view_.render();
    }

    void LoadingState::onPause() {

    }

    void LoadingState::handleEvent(sf::Event event) {
        std::lock_guard<std::mutex> lock(mtx_);
        view_.handleEvent(event);
    }

    void LoadingState::onResume() {

    }

    void LoadingState::onExit() {
        ime::EventDispatcher::instance()->dispatchEvent("resourceLoadingComplete");
    }
}
