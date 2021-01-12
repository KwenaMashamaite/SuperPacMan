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

#ifndef SUPERPACMAN_KEY_H
#define SUPERPACMAN_KEY_H

#include <IME/core/entity/Entity.h>
#include <IME/graphics/AnimatableSprite.h>

namespace pacman {
    /**
     * @brief A key that can lock or unlock a door
     *
     * @see Door
     */
    class Key : public ime::Entity {
    public:
        /**
         * @brief Construct the key
         * @param boundingRect The keys bounding rectangle dimensions
         * @param id Identification code of the key
         *
         * The id is useful for distinguishing between different key
         * objects. Multiple key objects can have the same identifier
         */
        Key(const ime::Vector2u &boundingRect, int id);

        /**
         * @brief Copy constructor
         */
        Key(const Key&) = default;

        /**
         * @brief Move constructor
         */
        Key(Key&&) = default;

        /**
         * @brief Assignment operator
         */
        Key& operator=(const Key& rhs) = default;

        /**
         * @brief Get the keys identification code
         * @return The keys identification code
         */
        int getId() const;

        /**
         * @brief Get the class type
         * @return Name of the concrete class the key belongs to
         */
        std::string getClassType() override;

        /**
         * @brief Get the sprites graphical representation
         * @return The sprites graphical representation
         */
        ime::Sprite& getSprite();

    private:
        int id_;                       //!< The keys id
        ime::Sprite sprite_; //!< Keys graphical representation
    };
}

#endif
