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

#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

#include <IME/graphics/Sprite.h>
#include <unordered_map>

namespace SuperPacMan {
    /**
     * @brief Container for storing static sprites that can be retrieved later.
     *        The container is accessible from anywhere in the program
     */
    class SpriteContainer {
    public:
        /**
         * @brief Add a sprite to the container
         * @param name Unique identifier of the sprite
         * @param sprite Sprite to be added
         */
        static void addSprite(const std::string& name, IME::Graphics::Sprite sprite);

        /**
         * @brief Get a copy of a sprite from the container
         * @param name Unique identifier of the sprite
         * @return The sprite with the specified identifier
         *
         * @warning The sprite must exist in the container before this
         * function is called, otherwise the behavior is undefined
         */
        static IME::Graphics::Sprite getSprite(const std::string& name);

        /**
         * @brief Check if the container has a sprite with a certain identifier
         * @param name Unique identifier of the sprite
         * @return True if the sprites exists, otherwise false
         */
        static bool hasSprite(const std::string& name);

        /**
         * @brief Remove a sprite from the container
         * @param name Unique identifier that was used when the sprite was added
         * @return True if the sprite was removed, or false if the container
         *         does not have a sprite with the specified name
         */
        static bool removeSprite(const std::string& name);

    private:
        inline static std::unordered_map<std::string, IME::Graphics::Sprite> sprites_{};
    };
}

#endif
