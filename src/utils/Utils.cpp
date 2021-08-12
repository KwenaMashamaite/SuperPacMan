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

#include "src/utils/Utils.h"
#include "src/models/actors/Door.h"
#include "src/models/actors/Key.h"
#include <cassert>

namespace spm::utils {
    namespace {
        /**
         * @brief Get the key id that will open a door at a specific index
         * @param doorIndex Index of the door
         * @return The keys id
         *
         * The door is always opened by a key with an id that matches the doors
         * locker. As such, Key-locker combinations are hardcoded, this means that
         * a certain key will always open specific doors, regardless of where the
         * key it is placed
         */
        int getLockerId(ime::Index doorIndex);
    } // namespace anonymous

    ///////////////////////////////////////////////////////////////
    std::string convertToString(ime::Direction direction) {
        if (direction == ime::Left)
            return "Left";
        else if (direction == ime::Right)
            return "Right";
        else if (direction == ime::Up)
            return "Up";
        else if (direction == ime::Down)
            return "Down";
        else
            return "Unknown";
    }

    ///////////////////////////////////////////////////////////////
    bool unlockDoor(Door* door, const Key* key) {
        if (!door->isLocked())
            return false;

        door->unlockWith(*key);
        if (!door->isLocked())
            return true;

        return false;
    }

    ///////////////////////////////////////////////////////////////
    void lockDoor(Door* door) {
        auto index = door->getUserData().getValue<ime::Index>("gridIndex");
        door->addDoorLocker(DoorLocker(getLockerId(index)));
        door->lockWith(Key(door->getUserData().getValue<std::reference_wrapper<ime::Scene>>("scene"), getLockerId(index)));
    }

    ///////////////////////////////////////////////////////////////
    std::string getFruitName(int level) {
        auto static fruitName = std::vector{
            "apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach",
            "melon", "coffee", "mushroom", "bell", "clover", "galaxian", "gift"
        };

        if (level <= fruitName.size())
            return fruitName.at(level - 1);

        assert(false && (std::string("Level \"") + std::to_string(level) + "\" is unsupported").c_str());
        return ""; // Silence warning: Non-void function does not return a value in all control paths
    }

    namespace {
        ///////////////////////////////////////////////////////////////
        int getLockerId(ime::Index doorIndex) {
            if (doorIndex == ime::Index{4, 5} || doorIndex == ime::Index{6, 5})
                return 0;
            else if (doorIndex == ime::Index{4, 21} || doorIndex == ime::Index{6, 21})
                return 1;
            else if (doorIndex == ime::Index{5, 8} || doorIndex == ime::Index{4, 13})
                return 2;
            else if (doorIndex == ime::Index{5, 18})
                return 3;
            else if (doorIndex == ime::Index{10, 5} || doorIndex == ime::Index{13, 4} || doorIndex == ime::Index{13, 6})
                return 4;
            else if (doorIndex == ime::Index{10, 21} || doorIndex == ime::Index{13, 20} || doorIndex == ime::Index{13, 22})
                return 5;
            else if (doorIndex == ime::Index{16, 5} || doorIndex == ime::Index{20, 5})
                return 6;
            else if (doorIndex == ime::Index{13, 8} || doorIndex == ime::Index{16, 9} || doorIndex == ime::Index{17, 12})
                return 7;
            else if (doorIndex == ime::Index{13, 18} || doorIndex == ime::Index{17, 14} || doorIndex == ime::Index{16, 17})
                return 8;
            else if (doorIndex == ime::Index{16, 21} || doorIndex == ime::Index{20, 21})
                return 9;
            else if (doorIndex == ime::Index{22, 13} || doorIndex == ime::Index{13, 2} || doorIndex == ime::Index{13, 24})
                return 10;
            else if (doorIndex == ime::Index{22, 9} || doorIndex == ime::Index{25, 8} || doorIndex == ime::Index{24, 13})
                return 11;
            else if (doorIndex == ime::Index{22, 17} || doorIndex == ime::Index{25, 18})
                return 12;
            else if (doorIndex == ime::Index{22, 5} || doorIndex == ime::Index{24, 5})
                return 13;
            else if (doorIndex == ime::Index{22, 21} || doorIndex == ime::Index{24, 21})
                return 14;
            else return -1;
        }
    } // namespace anonymous

} // namespace spm
