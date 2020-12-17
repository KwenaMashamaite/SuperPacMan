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
#include <IME/graphics/ui/widgets/ProgressBar.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <thread>
#include <chrono>

//Warning!! This number must be updated each time a new resource is added to the
//resources to be loaded
const auto numOfResources = 15;

namespace SuperPacMan {
    LoadingState::LoadingState(IME::Engine &engine) :
        State(engine),
        isInitialized_{false},
        view_(engine.getRenderTarget().getSize()),
        loadingFinished_{false}
    {}

    void LoadingState::initialize() {
        view_.init();
        view_.getWidget<IME::UI::ProgressBar>("loadingProgressBar")->setMaximumValue(numOfResources);

        // Transition to next state if all resources are loaded
        engine().onFrameEnd([this] {
            if (loadingFinished_) {
                engine().popState();
                engine().onFrameEnd(nullptr);
            }
        });

        //Initiate resource loading
        std::thread([this] {
            loadResources();
        }).detach();

        isInitialized_ = true;
    }

    void LoadingState::loadResources() {
        auto updateProgressBar = [this](const std::string& text) {
            static auto loadingProgressBar = view_.getWidget<IME::UI::ProgressBar>("loadingProgressBar");
            //Resources load very fast (less than a second), so we simulate a delay (= numberOfResources * threadSleepTime)
            std::this_thread::sleep_for(std::chrono::milliseconds (200));
            std::lock_guard<std::mutex> lock(mtx_);
            loadingProgressBar->setText("Loading " + text + "...");
            loadingProgressBar->incrementValue();
        };

        //LOAD FONTS
        std::unique_lock<std::mutex> lock(mtx_);
        view_.getWidget<IME::UI::IWidget>("loadingText")->setText("Loading fonts...");
        lock.unlock();
        IME::ResourceManager::getInstance()->loadFromFile(IME::ResourceType::Font, {
            "namco.ttf", "AtariClassicExtrasmooth-LxZy.ttf"}, updateProgressBar);

        //LOAD TEXTURES
        lock.lock();
        view_.getWidget<IME::UI::Label>("loadingText")->setText("Loading textures...");
        lock.unlock();
        IME::ResourceManager::getInstance()->loadFromFile(IME::ResourceType::Texture,  {
            "icon.png", "grids.png", "pacman_logo.png", "spritesheet.png"
        }, updateProgressBar);

        //LOAD SOUND EFFECTS
        lock.lock();
        view_.getWidget<IME::UI::Label>("loadingText")->setText("Loading sound effects...");
        lock.unlock();
        IME::ResourceManager::getInstance()->loadFromFile(IME::ResourceType::SoundBuffer, {
            "doorBroken.wav", "fruitEaten.wav", "ghostEaten.wav", "pacmanDying.wav",
            "powerPelletEaten.wav", "superPelletEaten.wav", "beginning.wav"
        }, updateProgressBar);

        //LOAD MUSIC
        lock.lock();
        view_.getWidget<IME::UI::Label>("loadingText")->setText("Loading music...");
        lock.unlock();
        IME::ResourceManager::getInstance()->loadFromFile(IME::ResourceType::Music, {
            "searching.ogg", "pacman_intermission.ogg"
        }, updateProgressBar);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        loadingFinished_ = true;
    }

    void LoadingState::update(float deltaTime) {

    }

    void LoadingState::fixedUpdate(float deltaTime) {

    }

    void LoadingState::render(IME::Graphics::Window &renderTarget) {
        std::lock_guard<std::mutex> lock(mtx_);
        view_.render(renderTarget);
    }

    void LoadingState::pause() {

    }

    void LoadingState::handleEvent(sf::Event event) {
        std::lock_guard<std::mutex> lock(mtx_);
        view_.handleEvent(event);
    }

    void LoadingState::resume() {

    }

    bool LoadingState::isInitialized() const {
        return isInitialized_;
    }

    void LoadingState::exit() {
        IME::EventDispatcher::instance()->dispatchEvent("resourceLoadingComplete");
    }
}
