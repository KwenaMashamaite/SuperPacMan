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

#include <IME/core/game_object/GameObject.h>

namespace spm {
    /**
     * @brief Ghost actor
     */
    class Ghost : public ime::GameObject {
    public:
        /**
         * @brief The colour of the ghost (For Cosmetic purposes only)
         */
        enum class Colour {
            Red,     //!< Blinky
            Pink,    //!< Pinky
            Cyan,    //!< Inky
            Orange,  //!< Clyde
        };


        /**
         * @brief Constructor
         * @param scene The scene that the actor is in
         * @param colour The colour of the ghost
         */
        explicit Ghost(ime::Scene& scene, Colour colour);

        /**
         * @brief Get the name of this class
         * @return The name of this class
         */
        std::string getClassName() const override;
    };
}

#endif
