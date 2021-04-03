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

#ifndef SUPERPACMAN_DOORLOCKER_H
#define SUPERPACMAN_DOORLOCKER_H

#include "src/models/actors/Key.h"

namespace spm {
    /**
     * @brief A class for locking doors
     *
     * When a door is fitted with a locker, it can be locked and
     * unlocked using a Key
     */
    class DoorLocker {
    public:
        using Ptr = std::shared_ptr<DoorLocker>; //!< Shared actor pointer

        /**
         * @brief Construct a door locker
         * @param id Identifier of the locker
         *
         * The identifier determines which keys can interact with this
         * locker. Only keys with the same id as this locker can lock
         * or unlock it
         */
        explicit DoorLocker(int id = -1);

        /**
         * @brief Set the id of the locker
         * @param id The new id of the locker
         */
        void setId(int id);

        /**
         * @brief Get the lockers id
         * @return The lockers id
         */
        int getId() const;

        /**
         * @brief Lock the locker with a key
         * @param key Key to lock the locker with
         *
         * The keys id must be the same as the lockers id in order for the
         * locker to be locked
         *
         * The locker is unlocked by default
         */
        void lock(const Key& key);

        /**
         * @brief Unlock the locker with a key
         * @param key Key to unlock the locker with
         *
         * The keys id must be the same as the lockers id in order for
         * the locker to be unlocked
         *
         * The locker is unlocked by default
         */
        void unlock(const Key& key);

        /**
         * @brief Check whether or not the ocker is locked
         * @return True if the locker is locked, otherwise false
         */
        bool isLocked() const;

    private:
        int id_;        //!< The lockers id
        bool isLocked_; //!< A flag indicating whether or not the locker is locked
    };
}

#endif
