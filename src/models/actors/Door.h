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

#ifndef SUPERPACMAN_DOOR_H
#define SUPERPACMAN_DOOR_H

#include <IME/core/game_object/GameObject.h>
#include "src/models/actors/DoorLocker.h"

namespace spm {
    /**
     * @brief Orientation of the door
     */
    enum class Orientation {
        Unknown,   //!< Unspecified orientation
        Vertical,  //!< Vertical orientation
        Horizontal //!, Horizontal orientation
    };

    /**
     * @brief A door that locks fruits and pellets away from Pacman
     */
    class Door : public ime::GameObject {
    public:
        using Ptr = std::shared_ptr<Door>; //!< Shared actor pointer

        /**
         * @brief Constructor
         * @param scene The scene the object belongs to
         */
        explicit Door(ime::Scene& scene);

        /**
         * @brief Set the orientation of the door
         * @param orientation Orientation of the door
         *
         * By default, the orientation is Orientation::Unknown
         */
        void setOrientation(Orientation orientation);

        /**
         * @brief Get the orientation of the door
         * @return The orientation of the door
         */
        Orientation getOrientation() const;

        /**
         * @brief Add a door locker
         * @param doorLocker Door locker to be added
         *
         * Without a door locker, the door cannot be locked. Pass
         * nullptr to remove the door locker
         *
         * By default, there is no door locker
         */
        void addDoorLocker(DoorLocker doorLocker);

        /**
         * @brief Lock the door with a key
         * @param key Key to lock door with
         *
         * The door can only be locked with a key that has the same id
         * as the installed door lockers id. This function has no effect
         * if the door does not have a locker installed or the door is
         * already locked
         *
         * @see addDoorLocker and unlockWith
         */
        void lockWith(const Key& key);

        /**
         * @brief unlock the door with a key
         * @param key Key to unlock door with
         *
         * The door can only be unlocked with a key that has the same id
         * as the installed door lockers id. This function has no effect if
         * the door does not have a locker installed or the door is already
         * unlocked
         *
         * @see addDoorLocker and lockWith
         */
        void unlockWith(const Key& key);

        /**
         * @brief Check whether or not the door is locked
         * @return True if door is locked or false if door is unlocked
         */
        bool isLocked() const;

        /**
         * @brief Force the door to open
         */
        void burstOpen();

        /**
         * @brief Check if the door was forced open or not
         * @return True if the door was forced open, otherwise false
         */
        bool isBroken() const;

        /**
         * @brief Get the name of this class
         * @return The name of this class
         */
        std::string getClassName() const override;

    private:
        /**
         * @brief Set the current texture of the door
         * @param alias The alias of a spritesheet tile to set as texture
         */
        void setDoorTexture(const std::string& alias);

    private:
        DoorLocker doorLocker_;        //!< Door locker
        Orientation orientation_;      //!< The current orientation of the door
        ime::SpriteSheet spriteSheet_; //!< Stores the different door state textures
        bool isBroken_;                //!< A flag indicating whether or not the door was forced open
    };
}

#endif
