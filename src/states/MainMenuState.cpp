#include "MainMenuState.h"
#include "PlayingState.h"
#include "LevelStartState.h"
#include <IME/core/loop/Engine.h>
#include <IME/utility/DiskFileReader.h>
#include <IME/graphics/ui/widgets/Button.h>
#include <IME/graphics/ui/widgets/BitmapButton.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/widgets/VerticalLayout.h>

using namespace IME::Graphics;
using namespace IME::Graphics::UI;

namespace SuperPacMan {
    MainMenuState::MainMenuState(IME::Engine &engine) :
        State(engine),
        isInitialized_(false),
        mainMenuView_(engine.getRenderTarget())
    {}

    void MainMenuState::initialize() {
        mainMenuView_.init(engine().getPersistentData().getValueFor<int>("high-score"));
        initUIButtonsBehavior();
        isInitialized_ = true;
    }

    void MainMenuState::initUIButtonsBehavior() {
        auto navButtonsContainer = mainMenuView_.getWidget<UI::VerticalLayout>("nav-btn-container");
        //PLAY BUTTON
        navButtonsContainer->getWidget("play-btn")->on("click", IME::Callback<>([this] {
            engine().popState();
            engine().pushState(std::move(std::make_shared<PlayingState>(engine())));
            engine().pushState(std::move(std::make_shared<LevelStartState>(engine())));
        }));

        //CONTROLS BUTTON
        auto text = std::stringstream();
        IME::Utility::DiskFileReader().readFileInto("textFiles/controls.txt", text);
        navButtonsContainer->getWidget("controls-btn")->on("click", IME::Callback<>([this, info = text.str()]{
            mainMenuView_.setSubView(SubView::Info);
            mainMenuView_.getWidget<UI::Label>("info-box")->setText(info);
        }));

        //EXIT BUTTON
        navButtonsContainer->getWidget("exit-btn")->on("click", IME::Callback<>([this]{
            engine().quit();
        }));

        //BACK BUTTON
        mainMenuView_.getWidget<UI::BitmapButton>("back-btn")->on("click", IME::Callback<>([this] {
            mainMenuView_.setSubView(SubView::Main);
            mainMenuView_.getWidget<UI::Label>("info-box")->setText("");
        }));
    }

    void MainMenuState::render(IME::Graphics::Window &renderTarget) {
        mainMenuView_.render(renderTarget);
    }

    void MainMenuState::handleEvent(sf::Event event) {
        mainMenuView_.handleEvent(event);
    }

    bool MainMenuState::isInitialized() const {
        return isInitialized_;
    }

    void MainMenuState::update(float deltaTime) {
        mainMenuView_.update(deltaTime);
    }

    void MainMenuState::fixedUpdate(float deltaTime) {

    }

    void MainMenuState::pause() {

    }

    void MainMenuState::resume() {

    }

    void MainMenuState::exit() {

    }
}
