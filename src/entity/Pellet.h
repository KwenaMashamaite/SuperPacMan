/**
 * @brief An eatable pellet
 */

#ifndef PELLET_H
#define PELLET_H

#include <IME/core/entity/Entity.h>
#include <IME/graphics/AnimatableSprite.h>
#include "IEatable.h"

namespace SuperPacMan {
    enum class PelletType {
        PowerPellet,
        SuperPellet
    };

    class Pellet : public IME::Entity, public IEatable {
    public:
        /**
         * @brief Construct a pellet object
         * @brief pelletType Type of the pellet
         * @param boundingRect Size of the objects bounding rectangle
         */
        explicit Pellet(PelletType pelletType, const IME::Vector2u &boundingRect);

        /**
         * @brief Eat pellet
         */
        void eat() override;

        /**
         * @brief Check if pellet is eaten or not
         * @return True if eaten or false if not eaten
         */
        bool isEaten() const override;

        /**
         * @brief Update pellet
         * @param deltaTime Time passed since pellet was last updated
         */
        void update(float deltaTime);

        /**
         * @brief Get the type of this pellet
         * @return The type of this pellet
         */
        PelletType getPelletType() const;

        /**
         * @brief Get the type of the object
         * @return The type of the object
         */
        std::string getObjectType() override;

        /**
         * @brief Get the pellets graphical representation
         * @return The pellets graphical representation
         */
        IME::Graphics::AnimatableSprite& getSprite();

    private:
        //The type of this pellet
        PelletType pelletType_;
        //Pellets graphical representation
        IME::Graphics::AnimatableSprite sprite_;
    };
}

#endif
