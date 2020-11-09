/**
 * @brief Defines the playing state of the game
 */

#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include <IME/core/states/State.h>
#include <IME/core/tilemap/TileMap.h>
#include <IME/graphics/ui/GuiContainer.h>

namespace SuperPacMan {
    class PlayingState : public IME::State {
    public:
        /**
         * @brief Create state
         * @param engine Reference to the game
         */
        explicit PlayingState(IME::Engine &engine);

        /**
         * @brief Initialize state
         */
        void initialize() override;

        /**
         * @brief Update the state
         * @param deltaTime Time passed since engine state was updates
         */
        void update(float deltaTime) override;

        /**
         * @brief Update the state
         * @param deltaTime Time passed since engine state was updates
         */
        void fixedUpdate(float deltaTime) override;

        /**
         * @brief Render the state on a render target
         * @param renderTarget Target to render state on
         */
        void render(IME::Graphics::Window &renderTarget) override;

        /**
         * @brief Pause the state
         */
        void pause() override;

        /**
         * @brief Handle event
         * @param event Event to be handled
         */
        void handleEvent(sf::Event event) override;

        /**
         * @brief Resume state
         */
        void resume() override;

        /**
         * @brief Check if state is initialized or not
         * @return True if initialized, otherwise false
         */
        bool isInitialized() const override;

        /**
         * @brief Perform clean up
         */
        void exit() override;

    private:
        /**
         * @brief Create the states grid
         */
        void createMaze();

        /**
         * @brief Create the current score and highscore displays
         */
        void createScoresText();

    private:
        //Initialization flag
        bool isInitialized_;
        //The grid of the game
        IME::TileMap tileMap_;
        //Container for the high score display
        IME::Graphics::UI::GuiContainer guiContainer_;
    };
}

#endif
