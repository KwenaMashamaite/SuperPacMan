/**
 * @brief Constructs all the pellet animations
 */

#ifndef PELLETANIMATIONS_H
#define PELLETANIMATIONS_H

#include <IME/core/animation/Animation.h>
#include <vector>
#include <memory>
#include "../entity/Pellet.h"

namespace SuperPacMan {
    class PelletAnimations {
    public:
        /**
         * @brief Create animations for a specific pellet
         * @param pelletType Pellet to create animations for
         */
        void createAnimationFor(PelletType pelletType);

        /**
         * @brief Get all of the created animations
         * @return A list of all the created animations
         */
        const std::vector<std::shared_ptr<IME::Animation>>& getAll() const;

    private:
        //Stores all the created animations
        std::vector<std::shared_ptr<IME::Animation>> animations_;
    };
}

#endif
