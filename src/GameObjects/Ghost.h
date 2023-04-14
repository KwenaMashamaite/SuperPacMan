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

#ifndef SUPERPACMAN_GHOST_H
#define SUPERPACMAN_GHOST_H

#include "AI/IActorState.h"
#include "Common/Events.h"
#include <Mighter2d/core/object/GridObject.h>
#include <memory>

namespace spm {
    /**
     * @brief Ghost actor
     */
    class Ghost : public mighter2d::GridObject {
    public:
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
            None = -1,    //!< No state
            Scatter,      //!< The ghost cycles a grid corner (cannot be eaten by Pacman)
            Chase,        //!< The ghost chases pacman (cannot be eaten by PacMan)
            Frightened,   //!< The ghost runs away from pacman (can be eaten by PacMan)
            Eaten         //!< The ghost retreats back to the ghost house (cannot be eaten by PacMan nor can it kill Pacman)
        };

        /**
         * @brief Constructor
         * @param scene The scene that the actor is in
         * @param colour The colour of the ghost
         */
        Ghost(mighter2d::Scene& scene, Colour colour);

        /**
         * @brief Get the name of this class
         * @return The name of this class
         */
        std::string getClassName() const override;

        /**
         * @brief Change the state
         * @param state The new state
         *
         * Any active state will be destroyed. Pass @a state as nullptr to
         * remove the current state
         */
        void setState(IActorState::Ptr state);

        /**
         * @brief Destroy the current state
         *
         * Similarly to passing nullptr as an argument to spm::Ghost::State,
         * this function removes the current state. However it does not call
         * the onExit function on the state before destroying it.
         */
        void clearState();

        /**
         * @brief Get the current state of the ghost
         * @return The current state of the ghost
         */
        State getState() const;

        /**
         * @brief Lock or unlock the ghost from the ghost house
         * @param lock True to lock the ghost or false to unlock the ghost
         *
         * By default, the ghost is not locked in the ghost house
         *
         * @warning The ghost must physically be in the ghost house before locking
         * it inside, otherwise undefined behavior
         */
        void setLockInGhostHouse(bool lock);

        /**
         * @brief Check if the ghost is locked in the ghost house or not
         * @return True if locked in the ghost house, otherwise false
         */
        bool isLockedInGhostHouse() const;

        /**
         * @brief Make the ghost flash or stop flashing
         * @param flash True to flash the ghost otherwise false
         *
         * Note, it can only flash when in the the spm::Ghost::State::Frightened state
         */
        void setFlash(bool flash);

        /**
         * @brief Check if the ghost is flashing
         * @return True if flashing, otherwise false
         */
        bool isFlashing() const;

        /**
         * @brief Flatten or unflatten the ghost
         * @param flatten True to flatten or false to unflatten
         */
        void setFlattened(bool flatten);

        /**
         * @brief Check if the ghost is flat or not
         * @return True if flat, otherwise false
         */
        bool isFlat() const;

        /**
         * @brief Update the ghost
         * @param deltaTime Time passed since last update
         */
        void update(mighter2d::Time deltaTime) override;

        /**
         * @brief Handle a game event
         * @param event The event to be handled
         * @param args Arguments associated with the event
         */
        virtual void handleEvent(GameEvent event, const mighter2d::PropertyContainer& args);

        /**
         * @brief Destructor
         */
        ~Ghost() override;

    private:
        /**
         * @brief Initialize Animations
         */
        void initAnimations();

    private:
        IActorState::Ptr state_;   //!< The current state of the ghost
        bool isLockedInHouse_;     //!< A flag indicating whether or not the ghost is locked in the ghost pen
        bool isFlat_;              //!< A flag indicating whether or not the ghost is flat
    };
}

#endif
