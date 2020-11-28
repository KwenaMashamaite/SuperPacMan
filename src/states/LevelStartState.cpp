#include "LevelStartState.h"
#include <IME/core/loop/Engine.h>

namespace SuperPacMan {
    LevelStartState::LevelStartState(IME::Engine &engine) :
        State(engine),
        view_(engine.getRenderTarget(), 1u, 3000, 50000),
        isInit_{false},
        stateTimeout_{1.0}
    {}

    void LevelStartState::initialize() {
        view_.init();
        isInit_ = true;
    }

    bool LevelStartState::isInitialized() const {
        return isInit_;
    }

    void LevelStartState::render(IME::Graphics::Window &renderTarget) {
        view_.render(renderTarget);
    }

    void LevelStartState::update(float deltaTime) {
        stateTimeout_ -=deltaTime;
        if (stateTimeout_ <= 0)
            engine().popState();
    }

    void LevelStartState::fixedUpdate(float deltaTime) {

    }

    void LevelStartState::pause() {

    }

    void LevelStartState::handleEvent(sf::Event event) {

    }

    void LevelStartState::resume() {

    }

    void LevelStartState::exit() {

    }
}