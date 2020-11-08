/**
 * @brief Container for storing static sprites that can be retrieved later.
 *        The container is accessible from anywhere in the program
 */

#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

#include <IME/graphics/Sprite.h>
#include <unordered_map>

namespace SuperPacMan {
    class SpriteContainer {
    public:
        /**
         * @brief Add a sprite to the container
         * @param name Name Unique identifier of the sprite
         * @param sprite Sprite to be added
         */
        static void addSprite(const std::string& name, IME::Graphics::Sprite sprite);

        /**
         * @brief Remove a sprite from the container
         * @param name Identifier that was used when the sprite was added
         * @return True if the sprite was removed, or false if the container
         *         does not have a sprite with the specified identifier
         */
        static bool removeSprite(const std::string& name);

        /**
         * @brief Check if the container has a sprite with a certain identifier
         * @param name Unique identifier of the sprite
         * @return True if the sprites exists, otherwise false
         */
        static bool hasSprite(const std::string& name);

        /**
         * @brief Get a copy of a sprite from the container
         * @param name Unique identifier of the sprite
         * @return The sprite with the specified identifier
         *
         * @note The sprite must exist in the container before this function is
         * called, otherwise the behavior is undefined
         */
        static IME::Graphics::Sprite getSprite(const std::string& name);

    private:
        //Stores the sprites
        inline static std::unordered_map<std::string, IME::Graphics::Sprite> sprites_{};
    };
}

#endif
