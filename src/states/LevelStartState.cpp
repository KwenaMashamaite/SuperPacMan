#include "LevelStartState.h"
#include <IME/core/loop/Engine.h>

namespace SuperPacMan {
    LevelStartState::LevelStartState(IME::Engine &engine) :
        State(engine),
        isInit_{false},
        stateTimeout_{2.0f}
    {}

    void LevelStartState::initialize() {
        auto level = engine().getPersistentData().getValueFor<int>("level");
        auto lives = engine().getPersistentData().getValueFor<int>("lives");
        auto score = engine().getPersistentData().getValueFor<int>("score");
        auto highScore = engine().getPersistentData().getValueFor<int>("high-score");
        view_ = std::make_unique<LevelStartView>(engine().getRenderTarget(), level, lives, score, highScore);
        view_->init();

        if (level == 1) { //Audio played for the first level only
            sfx_.play("beginning.wav");
            stateTimeout_ = sfx_.getDuration().Milliseconds / 1000.0f;
        }
        isInit_ = true;
    }

    bool LevelStartState::isInitialized() const {
        return isInit_;
    }

    void LevelStartState::render(IME::Graphics::Window &renderTarget) {
        view_->render(renderTarget);
    }

    void LevelStartState::update(float deltaTime) {
        view_->update(deltaTime);
        stateTimeout_ -= deltaTime;
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