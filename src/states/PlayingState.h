/**
 * @brief Defines the playing state of the game
 */

#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include <IME/core/states/State.h>
#include <IME/core/tilemap/TileMap.h>
#include <IME/graphics/ui/GuiContainer.h>
#include <IME/core/entity/Entity.h>
#include <IME/graphics/Sprite.h>
#include "../animators/IEntityAnimator.h"

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
        void createMaze();
        void createWalls();
        void createDoors();
        void createScoresText();
        void createFruits();
        void createKeys();
        void createPellets();

    private:
        //Aliases
        using EntitySpritePair = std::pair<std::shared_ptr<IME::Entity>, std::shared_ptr<IME::Graphics::Sprite>>;
        using EntityContainer = std::vector<EntitySpritePair>;

        //Initialization flag
        bool isInitialized_;
        //The current level
        unsigned int level_;
        //The grid of the game
        IME::TileMap tileMap_;
        //Container for the high score display
        IME::Graphics::UI::GuiContainer guiContainer_;
        //Container for all game entities
        std::unordered_map<std::string, EntityContainer> objects_;
        std::vector<std::shared_ptr<IEntityAnimator>> animators_;
        //Stores the types of fruit (Different fruit type each level)
        const std::vector<std::string> fruitType_{
            "apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake",
            "peach", "melon", "coffee", "mushroom", "bell", "clover", "galaxian",
            "gift"
        };
    };
}

#endif
