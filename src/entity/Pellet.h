/**
 * @brief An eatable pellet
 */

#ifndef PELLET_H
#define PELLET_H

#include <IME/core/entity/Entity.h>
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
         * @brief Get the type of this pellet
         * @return The type of this pellet
         */
        PelletType getPelletType() const;

        /**
         * @brief Get the type of the object
         * @return The type of the object
         */
        std::string getType() override;

    private:
        //The type of this pellet
        PelletType pelletType_;
    };
}

#endif
