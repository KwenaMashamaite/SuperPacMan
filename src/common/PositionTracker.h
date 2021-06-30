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

#ifndef SUPERPACMAN_POSITIONTRACKER_H
#define SUPERPACMAN_POSITIONTRACKER_H

#include <IME/core/tilemap/Index.h>
#include <string>
#include <unordered_map>

namespace spm {
    /**
     * @brief Tracks the position of an actor using its tag
     */
    class PositionTracker {
    public:
        /**
         * @brief Update the position of the actor
         * @param tag The tag of the actor whose position is to be updated
         * @param index The new position of the actor
         */
        static void updatePos(const std::string& tag, ime::Index index);

        /**
         * @brief Get the position of an actor
         * @param tag The tag of the actor
         * @return The position of the actor
         */
        static ime::Index getPos(const std::string& tag);

    private:
        inline static std::unordered_map<std::string, ime::Index> position_{};
    };
}

#endif //SUPERPACMAN_POSITIONTRACKER_H
