/**
 * @brief Models a door
 */

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
        Vertical,
        Horizontal
    };

    class Door : public IME::Entity {
    public:
        /**
         * @brief Create door
         * @param boundingRect Size of the rectangle surrounding the door object
         *
         * The door is unlocked by default and has a position of (0, 0)
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
         * @brief Set the orientation of the wall
         * @param orientation Orientation of the wall
         *
         * The door has a vertical orientation by default
         */
        void setOrientation(Orientation orientation);

        /**
         * @brief Add a door locker
         * @param doorLocker Door locker to add
         *
         * Without the door locker the door cannot be locked
         */
        void addDoorLocker(std::unique_ptr<DoorLocker> doorLocker);

        /**
         * @brief Lock the door with a key
         * @param key Key to lock door with
         *
         * The door can only be locked with a key that locks the doors locker
         * @see addLocker()
         */
        void lockWith(const Key& key);

        /**
         * @brief unlock the door with a key
         * @param key Key to unlock door with
         *
         * The door can only be unlocked with a key that unlocks the doors locker
         * @see addLocker()
         */
        void unlockWith(const Key& key);

        /**
         * @brief Check if door locked or not
         * @return True if door is locked or false if door is unlocked
         */
        bool isLocked() const;

        /**
         * @brief Add an event listener to a door locked event
         * @param callback Function to execute when the door is locked
         */
        int onLock(IME::Callback<> callback);

        /**
         * @brief Add an event listener to a door unlocked event
         * @param callback Function to execute when the door is unlocked
         */
        int onUnlock(IME::Callback<> callback);

        /**
         * @brief Force the door to open
         *
         * This function will break the doors locker if it has one, therefore
         * the door cannot be locked after it has been forced open. In order
         * for the door to be lockable again, the locker must be replaced
         * @see addDoorLocker()
         */
        void forceOpen();

        /**
         * @brief Get the orientation of the door
         * @return The orientation of the door
         */
        Orientation getOrientation() const;

        /**
         * @brief Get the type of the object
         * @return Type of the object
         */
        std::string getObjectType() override;

        /**
         * @brief Get the objects graphical representation
         * @return The objects graphical representation
         */
        IME::Graphics::Sprite& getSprite();

    private:
        //Door locker
        std::unique_ptr<DoorLocker> doorLocker_;
        //The orientation of the door
        Orientation orientation_;
        //Doors visual representation
        IME::Graphics::Sprite sprite_;
    };
}

#endif
