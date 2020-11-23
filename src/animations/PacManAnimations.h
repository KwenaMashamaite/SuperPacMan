/**
 * @brief Constructs all the pacman animations
 */

#ifndef PACMANANIMATIONS_H
#define PACMANANIMATIONS_H

#include <IME/core/animation/Animation.h>
#include <IME/common/Rect.h>
#include <vector>
#include <memory>

namespace SuperPacMan {
    class PacManAnimations {
    public:
        /**
         * @brief Create the pacman animations
         */
        void create();

        /**
         * @brief Get all of the created animations
         * @return A list of all the pacman animations
         */
        const std::vector<std::shared_ptr<IME::Animation>>& getAll() const;

    private:
        /**
         * @brief Create an animation
         * @param name Name of the animation
         * @param rect Animation frame
         */
        void createAnimation(const std::string& name, IME::IntRect rect);

    private:
        //Stores all the created animations
        std::vector<std::shared_ptr<IME::Animation>> animations_;
    };
}

#endif
