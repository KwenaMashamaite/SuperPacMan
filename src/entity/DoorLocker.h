/**
 * @brief Class for locking doors
 */

#ifndef DOORLOCKER_H
#define DOORLOCKER_H

#include "Key.h"

namespace SuperPacMan {
    class DoorLocker {
    public:
        /**
         * @brief Construct door locker object
         * @param id Identifier of the locker
         *
         * The identifier determines which keys can interact with this locker.
         * Only keys with the same id as this locker can lock or unlock it
         */
        explicit DoorLocker(int id);

        /**
         * @brief Lock the locker with a key
         * @param key Key to lock the locker with
         *
         * The locker is unlocked by default. The keys id must be the same as
         * the lockers id in order for the locker to be locked
         */
        void lock(const Key& key);

        /**
         * @brief Unlock the locker with a key
         * @param key Key to unlock the locker with
         *
         * The keys id must be the same as the lockers id in order for the
         * locker to be unlocked
         */
        void unlock(const Key& key);

        /**
         * @brief Check if locker is locked or not
         * @return True if the locker is locked, otherwise false
         */
        bool isLocked() const;

        /**
         * @brief Get the lockers id
         * @return The lockers id
         */
        int getId() const;

    private:
        //The lockers id
        int id_;
        //The lockers locked state
        bool isLocked_;
    };
}

#endif
