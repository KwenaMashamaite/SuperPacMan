/**
 * @brief Models a key that can open a door
 */

#ifndef KEY_H
#define KEY_H

#include <IME/core/entity/Entity.h>
#include <IME/graphics/AnimatableSprite.h>

namespace SuperPacMan {
    class Key : public IME::Entity {
    public:
        /**
         * @brief Construct the key
         * @param boundingRect The keys bounding rectangle dimensions
         * @param id Identification code of the key
         *
         * The id is useful for distinguishing between different key objects.
         * Multiple key objects can have the same identifier. By default the
         * key has a position of (0, 0)
         */
        Key(const IME::Vector2u &boundingRect, int id);

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
         * @brief Get the type of the object
         * @return The type of the object
         */
        std::string getObjectType() override;

        /**
         * @brief Get the sprites graphical representation
         * @return The sprites graphical representation
         */
        IME::Graphics::Sprite& getSprite();

    private:
        //Identifier that separates this key from other key objects
        int id_;
        //Keys graphical representation
        IME::Graphics::Sprite sprite_;
    };
}

#endif
