////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#ifndef SUPERPACMAN_GAMEPLAYSCENE_H
#define SUPERPACMAN_GAMEPLAYSCENE_H

#include <Mighter2d/core/scene/Scene.h>
#include "Grid.h"
#include "Managers/GameObjectsManager.h"
#include "Managers/AudioManager.h"
#include "GameplayObserver.h"
#include "Managers/TimerManager.h"
#include "Managers/CharacterMovementManager.h"
#include "Managers/InputManager.h"
#include "Managers/GameFlowManager.h"
#include "Managers/GuiManager.h"
#include "Managers/CollisionManager.h"
#include "Managers/ScoreManager.h"

namespace spm {
    /**
     * @brief Defines the playing state of the game
     */
    class GameplayScene : public mighter2d::Scene {
    public:
        /**
         * @brief Constructor
         */
        GameplayScene();

        /**
         * @brief Enter the scene
         *
         * This function is called by the game engine when the scene
         * is entered for the first time
         */
        void onStart() override;

        /**
         * @brief Get the current game level
         * @return The current game level
         */
        int getGameLevel() const;

        /**
         * @brief Check if game level is a bonus stage
         * @return True if its a bonus stage, otherwise false
         */
        bool isBonusStage() const;

        /**
         * @brief Get the gameplay scene gui
         * @return The gameplay scene gui
         */
        GuiManager& getGuiManager();

        /**
         * @brief Get the gameplay grid
         * @return The gamepley grid
         */
        Grid& getGrid();

        /**
         * @brief Get the game audio player
         * @return The game audio player
         */
        AudioManager& getAudioPlayer();

        /**
         * @brief Get the scenes game object manager
         * @return The scenes game objects manager
         */
        GameObjectsManager& getGameObjectsManager();

        /**
         * @brief Get the games timer manager
         * @return The game timer manager
         */
        TimerManager& getTimerManager();

        /**
         * @brief Get the games input manager
         * @return The input manager
         */
        InputManager& getInputManager();

        /**
         * @brief Get the game flow manager
         * @return The game flow manager
         */
        GameFlowManager& getGameFlowManager();

        /**
         * @brief Get the gameplay observer
         * @return The gameplay observer
         */
        GameplayObserver& getGameplayObserver();

        /**
         * @brief Destructor
         */
        ~GameplayScene() override;

    private:
        GameplayObserver gameplayObserver_;
        Grid grid_;
        GameObjectsManager gameObjectsManager_;
        InputManager inputManager_;
        GameFlowManager gameFlowManager_;
        CharacterMovementManager gameObjectMovementManager_;
        AudioManager audioManager_;
        TimerManager timerManager_;
        GuiManager guiManager_;
        CollisionManager collisionManager_;
        ScoreManager scoreManager_;
    };
}

#endif
