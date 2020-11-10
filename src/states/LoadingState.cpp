#include "LoadingState.h"
#include "IntroState.h"
#include <IME/core/loop/Engine.h>
#include <IME/graphics/ui/widgets/ProgressBar.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <thread>
#include <chrono>

//Warning!! This number must be updated each time a new resource is added to the
//resources to be loaded
const auto numOfResources = 16;

namespace SuperPacMan {
    LoadingState::LoadingState(IME::Engine &engine) : State(engine),
        guiContainer_(engine.getRenderTarget()), loadingFinished_{false}
    {}

    void LoadingState::initialize() {
        createTitle();
        createProgressBar();

        engine().onFrameEnd([this] {
            if (loadingFinished_) {
                engine().popState();
                engine().pushState(std::make_shared<IntroState>(engine()));
                engine().onFrameEnd(nullptr);
            }
        });

        //Initiate resource loading
        std::thread([this]{loadResources();}).detach();
    }

    void LoadingState::createTitle() {
        auto windowWidth = std::stof(engine().getSettings().getValueFor("windowWidth"));
        auto windowHeight = std::stof(engine().getSettings().getValueFor("windowHeight"));

        auto gameTitle = std::make_shared<IME::Graphics::UI::Label>(engine().getGameName());
        gameTitle->setTextSize(45.0f);
        gameTitle->scale(0, 1.5f);
        gameTitle->getRenderer()->setFont("pacfont.ttf");
        gameTitle->getRenderer()->setTextColour({255, 0, 0, 100});
        gameTitle->setOrigin(0.5f, 0.0f);
        gameTitle->setPosition(windowWidth / 2.0f, 50.0f);
        guiContainer_.addWidget(gameTitle, "gameTitle");

        mascot_.setTexture("mascot.png");
        mascot_.setOrigin(mascot_.getSize().width / 2.0f, mascot_.getSize().height / 2.0f);
        mascot_.scale(0.4f, 0.4f);
        mascot_.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);

        auto poweredByText = std::make_shared<IME::Graphics::UI::Label>("Powered by");
        poweredByText->getRenderer()->setTextStyle(IME::TextStyle::Italic);
        poweredByText->setOrigin(0.5f, 0.0f);
        poweredByText->setPosition(mascot_.getPosition().x, mascot_.getPosition().y + mascot_.getSize().height / 4.5f);
        guiContainer_.addWidget(poweredByText, "poweredBy");
        engineLogo_.setTexture("logo-IME.png");
        engineLogo_.setOrigin(engineLogo_.getSize().width / 2.0f, 0.0f);
        engineLogo_.scale(0.9f, 0.9f);
        engineLogo_.setPosition(poweredByText->getPosition().x, poweredByText->getPosition().y + poweredByText->getSize().height);
    }

    void LoadingState::createProgressBar() {
        auto windowSize = engine().getRenderTarget().getSize();
        auto loadingProgressBar = std::make_shared<IME::Graphics::UI::ProgressBar>();
        loadingProgressBar->setText("loading fonts...");
        loadingProgressBar->getRenderer()->setBorderColour({0, 230, 64, 135});
        loadingProgressBar->setSize(loadingProgressBar->getSize().width + 125.0f, loadingProgressBar->getSize().height);
        loadingProgressBar->setOrigin(0.5, 0.5);
        loadingProgressBar->setMaximumValue(numOfResources);
        guiContainer_.addWidget(loadingProgressBar, "loadingProgressBar");
        loadingProgressBar->on("full", IME::Callback<>([=]{
            guiContainer_.getWidget("loadingText")->setText("Resources loaded successfully");
            loadingProgressBar->setText("100%");
        }));

        auto loadingText = std::make_shared<IME::Graphics::UI::Label>("Loading resources, please wait...");
        loadingText->setOrigin(0.5, 0.5);
        loadingText->setPosition(windowSize.width / 2.0f, windowSize.height - 3 * loadingProgressBar->getSize().height);
        loadingProgressBar->setPosition(loadingText->getPosition().x, loadingText->getPosition().y + loadingText->getSize().height);
        guiContainer_.addWidget(std::move(loadingText), "loadingText");
    }

    void LoadingState::loadResources() {
        auto updateProgressBar = [this](const std::string& text) {
            //Resources load very fast (less than a second), so we simulate a delay (= numberOfResources * threadSleepTime)
            std::this_thread::sleep_for(std::chrono::milliseconds (200));
            static auto loadingProgressBar = std::dynamic_pointer_cast<IME::Graphics::UI::ProgressBar>(
                guiContainer_.getWidget("loadingProgressBar"));
            loadingProgressBar->setText("Loading " + text + "...");
            loadingProgressBar->incrementValue();
        };

        //LOAD FONTS
        guiContainer_.getWidget("loadingText")->setText("Loading fonts...");
        IME::ResourceManager::getInstance()->loadFromFile(IME::ResourceType::Font, {
            "namco.ttf", "pacfont.ttf"}, updateProgressBar);

        //LOAD TEXTURES
        guiContainer_.getWidget("loadingText")->setText("Loading textures...");
        IME::ResourceManager::getInstance()->loadFromFile(IME::ResourceType::Texture,  {
            "icon.png", "introGridBackground.png", "gameplayGridBackground.png",
            "logo.png", "spritesTileset.png"
        }, updateProgressBar);

        //LOAD SOUND EFFECTS
        guiContainer_.getWidget("loadingText")->setText("Loading sound effects...");
        IME::ResourceManager::getInstance()->loadFromFile(IME::ResourceType::SoundBuffer, {
            "doorBroken.wav", "fruitEaten.wav", "ghostEaten.wav", "pacmanDying.wav",
            "powerPelletEaten.wav", "superPelletEaten.wav"
        }, updateProgressBar);

        //LOAD MUSIC
        guiContainer_.getWidget("loadingText")->setText("Loading music...");
        IME::ResourceManager::getInstance()->loadFromFile(IME::ResourceType::Music, {
            "pacman_beginning.wav", "searching.ogg", "pacman_intermission.ogg"
        }, updateProgressBar);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        loadingFinished_ = true;
    }

    void LoadingState::update(float deltaTime) {

    }

    void LoadingState::fixedUpdate(float deltaTime) {

    }

    void LoadingState::render(IME::Graphics::Window &renderTarget) {
        guiContainer_.draw();
        renderTarget.draw(engineLogo_);
        renderTarget.draw(mascot_);
    }

    void LoadingState::pause() {

    }

    void LoadingState::handleEvent(sf::Event event) {
        guiContainer_.handleEvent(event);
    }

    void LoadingState::resume() {

    }

    bool LoadingState::isInitialized() const {
        return true;
    }

    void LoadingState::exit() {
        guiContainer_.removeAllWidgets();
    }
}

