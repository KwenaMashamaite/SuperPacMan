////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#ifndef SUPERPACMAN_GHOST_H
#define SUPERPACMAN_GHOST_H

#include "src/models/actors/states/ActorStateFSM.h"
#include <IME/core/game_object/GameObject.h>
#include <IME/core/physics/grid/GridMover.h>
#include <memory>
#include <src/common/Events.h>

namespace spm {
    /**
     * @brief Ghost actor
     */
    class Ghost : public ime::GameObject {
    public:
        using Ptr = std::unique_ptr<Ghost>; //!< Shared actor pointer

        /**
         * @brief The colour of the ghost
         */
        enum class Colour {
            Red,     //!< Blinky
            Pink,    //!< Pinky
            Cyan,    //!< Inky
            Orange,  //!< Clyde
        };

        /**
         * @brief States a ghost can be in at any given time
         */
        enum class State {
            Unknown = -1, //!< Unknown Ghost state
            Idle,         //!< The ghost remains stationary in at is current position (cannot be eaten by PacMan)
            Scatter,      //!< The ghost cycles a grid corner (cannot be eaten by Pacman)
            Chase,        //!< The ghost chases pacman (cannot be eaten by PacMan)
            Evade,        //!< The ghost runs away from pacman (can be eaten by PacMan)
            Heal,         //!< The ghost retreats back to the ghost house (cannot be eaten by PacMan nor can it kill Pacman)
            Wonder,       //!< The ghost moves randomly in the grid (cannot be eaten by PacMan)
        };

        /**
         * @brief Constructor
         * @param scene The scene that the actor is in
         * @param colour The colour of the ghost
         */
        Ghost(ime::Scene& scene, Colour colour);

        /**
         * @brief Initialize the ghosts Finite State Machine
         *
         * @note This function will replace all states present in the FSM with
         * the default ghost state
         */
        void initFSM();

        /**
         * @brief Set the movement controller of the ghost
         * @param gridMover The ghosts movement controller
         */
        void setMovementController(ime::GridMover* gridMover);

        /**
         * @brief Get the ghosts movement controller
         * @return The ghosts movement controller
         */
        ime::GridMover* getMoveController() const;

        /**
         * @brief Get the name of this class
         * @return The name of this class
         */
        std::string getClassName() const override;

        /**
         * @brief Get the current state of the ghost
         * @return The current state of the ghost
         *
         * @warning Calling this function before the ghosts FSM is initialized
         * is undefined behavior
         *
         * @see initFSM
         */
        State getState() const;

        /**
         * @brief Get the current direction
         * @return The current direction
         */
        ime::Vector2i getDirection() const;

        /**
         * @brief Update the ghost
         * @param deltaTime Time passed since last update
         */
        void update(ime::Time deltaTime) override;

        /**
         * @brief Handle a game event
         * @param event The event to be handled
         * @param args Arguments associated with the event
         */
        virtual void handleEvent(GameEvent event, const ime::PropertyContainer& args);

        /**
         * @brief Check if pacman is in super state or not
         * @return True if pacman is in super state, otherwise false
         *
         * @attention This function is a quick fix to a problem that requires
         * pacman-ghost interaction design reconsideration. spm::ChaseState
         * needs to know if pacman is in super state or not on enter. In an
         * attempt to keep spm::PacMan and spm::Ghost unaware of each other,
         * we let the ghost track pacmans state change via game events, see
         * handleEvent(). Now the spm::ChaseState queries the ghost about
         * pacmans super state via this function. Logically, this doesnt make
         * any sense. However due to time constrains, this function is in here
         * to push v1.0.0 release. As of this text, the game is in v1-beta.x
         *
         * @todo Remove this function post v1.0.0 release
         */
        bool isPacmanSuper() const;

        /**
         * @brief Destructor
         */
        ~Ghost() override;

    private:
        ime::GridMover* gridMover_;  //!< Controls the movement of the ghost in the grid
        ActorStateFSM fsm_;          //!< Ghosts finite state machine
        ime::Vector2i direction_;    //!< The direction of the ghost
        bool isPacmanSuper_;         //!< A flag indicating whether or not pacman is in super mode
    };
}

#endif
