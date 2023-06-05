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

#ifndef SUPERPACMAN_PACMAN_H
#define SUPERPACMAN_PACMAN_H

#include "AI/IActorState.h"
#include <Mighter2d/core/object/GridObject.h>

namespace spm {
    /**
     * @brief Player controlled actor
     */
    class PacMan : public mighter2d::GridObject {
    public:
        using Ptr = std::shared_ptr<PacMan>; //!< Shared actor pointer

        /**
         * @brief The state pacman can be in
         */
        enum class State {
            Normal,         //!< Pacman is normal size and can move around the maze (can be eaten by a Ghost)
            Super,          //!< Pacman is super sized and can move around the maze at an increased speed (cannot be eaten by a Ghost)
            Dying           //!< Pacman is dying and cannot be moved (cannot be eaten by a ghost)
        };

        /**
         * @brief Constructor
         * @param scene The scene the actor belongs to
         */
        explicit PacMan(mighter2d::Scene& scene);

        /**
         * @brief Set the number of lives
         * @param numOfLives New number of lives
         */
        void setLivesCount(int numOfLives);

        /**
         * @brief Get number of lives remaining
         * @return The number of lives remaining
         */
        int getLivesCount() const;

        /**
         * @brief Add one life
         */
        void addLife();

        /**
         * @brief Get the name of this class
         * @return The name of this class
         */
        std::string getClassName() const override;

        /**
         * @brief Change the state of Pacman
         * @param state The new state of pacman
         */
        void setState(State state);

        /**
         * @brief Get current state
         * @return The current state
         */
        State getState() const;

        /**
         * @brief Make pacman startFlash or stop flashing
         * @param flash True to startFlash, otherwise false
         *
         * Note, pacman flashes when in the the spm::PacMan::State::Super state only
         */
        void setFlashEnable(bool flash);

        /**
         * @brief Check if the pacman is flashing
         * @return True if flashing, otherwise false
         */
        bool isFlashing() const;

        /**
         * @brief Handle a game event
         * @param event The event to be handled
         */
        void handleEvent(GameEvent event);

        /**
         * @brief Change pacmans current animation
         * @param dir Pacmans current direction
         */
        void switchAnimation(mighter2d::Vector2i dir);

        /**
         * @brief Destructor
         */
        ~PacMan();

    private:
        /**
         * @brief @brief Initialize animations
         */
        void initAnimations();

    private:
        int livesCount_;             //!< The actors current number of lives
    };
}

#endif
