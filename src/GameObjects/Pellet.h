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

#ifndef SUPERPACMAN_PELLET_H
#define SUPERPACMAN_PELLET_H

#include <IME/core/game_object/GameObject.h>

namespace spm {
    /**
     * @brief A pellet that can be eaten by Pacman
     */
    class Pellet : public ime::GameObject {
    public:
        using Ptr = std::shared_ptr<Pellet>; //!< Shared actor pointer

        /**
         * @brief Pellet type
         */
        enum class Type {
            Power, //!< Makes ghosts vulnerable when eaten
            Super  //!< Gives Pacman super powers when eaten
        };

        /**
         * @brief Constructor
         * @param scene The scene the object is in
         * @param type The type of the pellet
         */
        Pellet(ime::Scene& scene, Type type);

        /**
         * @brief Get the class type
         * @return Name of the concrete class the pellet belongs to
         */
        std::string getClassName() const override;
    };
}

#endif
