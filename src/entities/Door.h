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

#ifndef DOOR_H
#define DOOR_H

#include <IME/core/entity/Entity.h>
#include "DoorLocker.h"
#include <memory>

namespace SuperPacMan {
    /**
     * @brief Orientation of the door
     */
    enum class Orientation {
        Vertical,  //!< Vertical orientation
        Horizontal //!, Horizontal orientation
    };

    /**
     * @brief A door that locks fruits and pellets away from Pacman
     */
    class Door : public IME::Entity {
    public:
        /**
         * @brief Create door
         * @param boundingRect Size of the rectangle surrounding the door object
         */
        explicit Door(const IME::Vector2u &boundingRect);

        /**
         * @brief Copy constructor
         */
        Door(const Door& other);

        /**
         * @brief Move constructor
         */
        Door(Door&& other) noexcept;

        /**
         * @brief Assignment operator
         */
        Door& operator=(Door rhs);

        /**
         * @brief Set the orientation of the door
         * @param orientation Orientation of the door
         *
         * The door has a vertical orientation by default
         */
        void setOrientation(Orientation orientation);

        /**
         * @brief Get the orientation of the door
         * @return The orientation of the door
         */
        Orientation getOrientation() const;

        /**
         * @brief Add a door locker
         * @param doorLocker Door locker to add
         *
         * Without the door locker the door cannot be locked. Pass
         * nullptr to remove the door locker. The door does not
         * have a locker installed by default
         */
        void addDoorLocker(std::unique_ptr<DoorLocker> doorLocker);

        /**
         * @brief Lock the door with a key
         * @param key Key to lock door with
         *
         * The door can only be locked with a key that has the same id
         * as the installed door locker. This function has no effect if
         * the door does not have a locker installed
         *
         * @see addDoorLocker
         */
        void lockWith(const Key& key);

        /**
         * @brief unlock the door with a key
         * @param key Key to unlock door with
         *
         * The door can only be unlocked with a key that has the same id
         * as the installed door locker. This function has no effect if
         * the door does not have a locker installed
         *
         * @see addDoorLocker
         */
        void unlockWith(const Key& key);

        /**
         * @brief Check if door is locked or not
         * @return True if door is locked or false if door is unlocked
         */
        bool isLocked() const;

        /**
         * @brief Force the door to open
         *
         * This function will break the doors locker if it has one, therefore
         * the door cannot be locked after it has been forced open. In order
         * for the door to be lockable again, the locker must be replaced
         *
         * @see addDoorLocker
         */
        void forceOpen();

        /**
         * @brief Add an event listener to a door lock event
         * @param callback Function to execute when the door is locked
         */
        int onLock(IME::Callback<> callback);

        /**
         * @brief Add an event listener to a door unlock event
         * @param callback Function to execute when the door is unlocked
         */
        int onUnlock(IME::Callback<> callback);

        /**
         * @brief Get the class type
         * @return Name of the concrete class the door belongs to
         */
        std::string getClassType() override;

        /**
         * @brief Get the doors graphical representation
         * @return The doors graphical representation
         */
        IME::Graphics::Sprite& getSprite();

    private:
        std::unique_ptr<DoorLocker> doorLocker_; //!< Door locker
        Orientation orientation_;                //!< The current orientation of the door
        IME::Graphics::Sprite sprite_;           //!< The doors graphical representation
    };
}

#endif
