#include "StartUpState.h"
#include "IntroState.h"
#include <IME/core/loop/Engine.h>

namespace SuperPacMan {
    StartUpState::StartUpState(IME::Engine &engine) : State(engine),
        grid_(20, 20), stateTimeout_{2.0f}
    {}

    void StartUpState::initialize() {
        grid_.loadFromFile("textFiles/levels/introMaze.txt");
        //The grid is visible by default, no need to call any setters
    }

    void StartUpState::update(float deltaTime) {
        stateTimeout_ -= deltaTime;
        if (stateTimeout_ <= 0.0f) {
            engine().popState();
            engine().pushState(std::make_shared<IntroState>(engine()));
        }
    }

    void StartUpState::fixedUpdate(float deltaTime) {

    }

    void StartUpState::render(IME::Graphics::Window &renderTarget) {
        grid_.draw(renderTarget);
    }

    void StartUpState::pause() {

    }

    void StartUpState::handleEvent(sf::Event event) {

    }

    void StartUpState::resume() {

    }

    bool StartUpState::isInitialized() const {
        return grid_.getSize().width > 0;
    }

    void StartUpState::exit() {

    }
}

