#include "MainMenuState.h"
#include "PlayingState.h"
#include <IME/core/loop/Engine.h>
#include <IME/utility/DiskFileReader.h>
#include <IME/graphics/ui/widgets/Button.h>
#include <IME/graphics/ui/widgets/BitmapButton.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/layout/VerticalLayout.h>

using namespace IME::Graphics;
using namespace IME::Graphics::UI;

namespace SuperPacMan {
    MainMenuState::MainMenuState(IME::Engine &engine)
        : State(engine), isInitialized_(false), mainViewContainer_{engine.getRenderTarget()},
          infoViewContainer_{engine.getRenderTarget()}, currentView_(View::Main)
    {}

    void MainMenuState::initialize() {
        createLogo();
        createUIElements();
        initUIButtonsBehavior();
        isInitialized_ = true;
    }

    void MainMenuState::createLogo() {
        auto windowWidth = std::stof(engine().getSettings().getValueFor("windowWidth"));
        auto windowHeight = std::stof(engine().getSettings().getValueFor("windowHeight"));

        logo_.setTexture("logo.png");
        logo_.setOrigin(logo_.getSize().width / 2.0f, 0.0f);
        logo_.scale(0.75, 0.75);
        logo_.setPosition(windowWidth / 2.0f, windowHeight * 6.0f / 100.0f);
    }

    void MainMenuState::createUIElements() {
        auto windowWidth = std::stof(engine().getSettings().getValueFor("windowWidth"));
        auto windowHeight = std::stof(engine().getSettings().getValueFor("windowHeight"));

        //Create Buttons
        struct ButtonDetails{std::string name; std::string text;};
        auto navigationButtons = std::vector<ButtonDetails>{
            {"play-btn", "PLAY"},
            {"controls-btn", "CONTROLS"},
            {"options-btn", "OPTIONS"},
            {"exit-btn", "EXIT"}
        };
        auto buttonsContainer = std::make_shared<VerticalLayout>(120, 180);
        buttonsContainer->setOrigin(0.5, 0.5);
        buttonsContainer->getRenderer()->setSpaceBetweenWidgets(20.0f);
        std::for_each(navigationButtons.begin(), navigationButtons.end(), [&](auto& buttonInfo) {
            auto button = std::make_shared<UI::Button>(buttonInfo.text);
            button->setTextSize(15.0f);
            button->getRenderer()->setBackgroundColour(Colour::Transparent);
            button->getRenderer()->setBorderColour(Colour::Transparent);
            button->getRenderer()->setBackgroundHoverColour(Colour::Transparent);
            button->getRenderer()->setTextColour(Colour::Red);
            button->getRenderer()->setTextHoverColour(Colour::Grey);
            buttonsContainer->addWidget(std::move(button), buttonInfo.name);
        });
        buttonsContainer->setPosition(windowWidth / 2.0f, windowHeight / 2.0f);
        mainViewContainer_.addWidget(std::move(buttonsContainer), "nav-btn-container");

        //Create text block for showing information when a main menu button is clicked
        auto infoBox = std::make_shared<Label>();
        infoBox->setOrigin(0.5, 0.5);
        infoBox->setPosition(windowWidth / 2.0f, windowWidth / 2.0f);
        infoViewContainer_.addWidget(std::move(infoBox), "info-box");

        //Create button for returning to the main menu from the info panel
        auto backButton = std::make_shared<UI::BitmapButton>("< Back");
        backButton->setTextSize(10.0f);
        backButton->on("click", IME::Callback<>([this] {
            currentView_ = View::Main;
        }));
        infoViewContainer_.addWidget(std::move(backButton), "back-btn");
    }

    void MainMenuState::initUIButtonsBehavior() {
        auto navButtonsContainer = std::dynamic_pointer_cast<UI::VerticalLayout>(mainViewContainer_.getWidget("nav-btn-container"));
        //PLAY BUTTON
        navButtonsContainer->getWidget("play-btn")->on("click", IME::Callback<>([this] {
            engine().popState();
            engine().pushState(std::move(std::make_shared<PlayingState>(engine())));
        }));

        //CONTROLS BUTTON
        auto text = std::stringstream();
        IME::Utility::DiskFileReader().readFileInto("textFiles/controls.txt", text);
        navButtonsContainer->getWidget("controls-btn")->on("click", IME::Callback<>([this, info = text.str()]{
            currentView_ = View::Info;
            infoViewContainer_.getWidget("info-box")->setText(info);
        }));

        //EXIT BUTTON
        navButtonsContainer->getWidget("exit-btn")->on("click", IME::Callback<>([this]{
            engine().quit();
        }));
    }

    void MainMenuState::render(IME::Graphics::Window &renderTarget) {
        switch (currentView_) {
            case View::Main:
                renderTarget.draw(logo_);
                mainViewContainer_.draw();
                break;
            case View::Info:
                infoViewContainer_.draw();
                break;
        }
    }

    void MainMenuState::handleEvent(sf::Event event) {
        if (currentView_ == View::Main)
            mainViewContainer_.handleEvent(event);
        else
            infoViewContainer_.handleEvent(event);
    }

    bool MainMenuState::isInitialized() const {
        return isInitialized_;
    }

    void MainMenuState::update(float deltaTime) {

    }

    void MainMenuState::fixedUpdate(float deltaTime) {

    }

    void MainMenuState::pause() {

    }

    void MainMenuState::resume() {

    }

    void MainMenuState::exit() {
        mainViewContainer_.removeAllWidgets();
        infoViewContainer_.removeAllWidgets();
    }
}
