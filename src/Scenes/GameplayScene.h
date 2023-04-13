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

#include <IME/core/scene/Scene.h>
#include "Common/Events.h"
#include "Grid.h"
#include "Views/CommonView.h"
#include "CollisionResponseRegisterer.h"
#include "Managers/TimerManager.h"
#include "Managers/GameObjectsManager.h"

namespace spm {
    /**
     * @brief Defines the playing state of the game
     */
    class GameplayScene : public ime::Scene {
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
        void onEnter() override;

        /**
         * @brief
         *
         * This function is called by the game engine when the game transitions
         * to another scene without destroying the current active scene
         */
        void onPause() override;

        /**
         * @brief Restart level after pacman dies
         *
         * This function is called by the game engine when the game returns
         * to this scene from another one
         */
        void onResume() override;

        /**
         * @brief Update the scene
         * @param deltaTime Time passed since last update
         *
         * This function is called by the game engine after the scene
         * has handled system events and external inputs (Keyboard and
         * mouse). The function is called once per frame and the delta
         * passed to it is frame rate dependent
         */
        void onUpdate(ime::Time deltaTime) override;

        /**
         * @brief Handle cache reactivation
         *
         * This function is called by the game engine when the scene is
         * resumed from the cache
         */
        void onResumeFromCache() override;

        /**
         * @brief Restore engine defaults
         *
         * This function is called by the game engine before the scene
         * is destroyed
         */
        void onExit() override;

        /**
         * @brief Handle frame end event
         *
         * This function is called by the game engine when the current
         * frame ends
         */
        void onFrameEnd() override;

        /**
         * @brief Emit a game event
         * @param event The event to be emitted
         */
        void emit(GameEvent event);

        int getLevel() const {return currentLevel_; }

        /**
         * @brief Destructor
         */
        ~GameplayScene();

        int pointsMultiplier_;

        GameObjectsManager gameObjectsManager_;
    private:
        /**
         * @brief Initialize the gui
         */
        void initGui();

        /**
         * @brief Create the gameplay grid
         */
        void initGrid();

        /**
         * @brief Create movement controllers for pacman and ghosts
         */
        void initMovementControllers();

        /**
         * @brief Initialize pacmans collision responses
         *
         * This function defines what happens when pacman collides with
         * a key, pellet, fruit or a ghost
         */
        void initCollisions();

        /**
         * @brief Initialize gameplay scene events
         *
         * These events only trigger when this state is active
         */
        void initSceneLevelEvents();

        /**
         * @brief Initialize game engine events
         *
         * These events will trigger in any state
         */
        void initEngineLevelEvents();

        /**
         * @brief Update cache and view score values
         * @param points The points to update the score by
         */
        void updateScore(int points);

        /**
         * @brief Start the level start countdown
         *
         * This function replaces the "Get Ready!!" text with a countdown
         * from 3 to 1. When the countdown reaches 0, the function emits a
         * spm::GameEvent::LevelStarted event and the gameplay begins
         */
        void initLevelStartCountdown();

        /**
         * @brief Reset pacman and the ghosts to their respective spawm tiles
         */
        void resetActors();

        /**
         * @brief Transition game to pause menu
         */
        void pauseGame();

        /**
         * @brief Resume gameplay
         */
        void resumeGame();

        /**
         * @brief Restart the current game level without resetting state
         */
        void resetLevel();

        /**
         * @brief Update the ghost point multiplier
         *
         * The multiplier increases by a factor of 2 every time the player
         * eats a ghost. However, since the player cannot eat more than four
         * ghosts in a single power mode session, the multiplier is capped to
         * 8. After the player eats the 4th ghost, the multiplier resets
         * to 1 (default). It also resets to 1 when power mode expires
         */
        void updatePointsMultiplier();

        /**
         * @brief End the gameplay
         *
         * This function is called when the player loses all lives
         */
        void endGameplay();

    private:
        int currentLevel_;              //!< Current game level
        bool isPaused_;                 //!< A flag indicating whether or not the game is paused
        CommonView* view_;               //!< Scene view without the gameplay grid
        std::unique_ptr<Grid> grid_;    //!< Gameplay grid view
        AudioManager audioManager_;     //!< Games audio manager
        int onWindowCloseId_;           //!< The id number of the 'onClose' event handler
        bool isChaseMode_;              //!< A flag indicating whether or not ghosts are in chase mode
        bool starAppeared_;             //!< A flag indicatinig whether or not a star has already been spawned
        bool isBonusStage_;             //!< A counter indicating whether or not the current level is a bonus stage
        CollisionResponseRegisterer collisionResponseRegisterer_;
        TimerManager timerManager_;

        friend class CollisionResponseRegisterer;
        friend class GameObjectsManager;
    };
}

#endif
