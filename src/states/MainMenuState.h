/**
 * @brief Defines the main menu state of the game
 */

#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <IME/core/states/State.h>
#include <IME/graphics/ui/GuiContainer.h>
#include <IME/graphics/Sprite.h>
#include "../view/MainMenuView.h"

namespace SuperPacMan {
    class MainMenuState : public IME::State {
    public:
        /**
         * @brief Create state
         * @param engine Reference to the game
         */
        explicit MainMenuState(IME::Engine &engine);

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
         * @brief Create the game logo
         */
        void createLogo();

        /**
         * @brief Create the main menu buttons and text labels
         */
        void createUIElements();

        /**
         * @brief Initialize the behavior of the main menu buttons
         */
        void initUIButtonsBehavior();

    private:
        //Initialization flag
        bool isInitialized_;
        //
        MainMenuView mainMenuView_;
    };
}

#endif
