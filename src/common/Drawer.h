/**
 * @brief Class for rendering objects
 */

#ifndef DRAWER_H
#define DRAWER_H

#include <vector>
#include <IME/core/entity/Entity.h>
#include <IME/graphics/Window.h>
#include <memory>

namespace SuperPacMan {
    class Drawer {
    public:
        /**
         * @brief Constructor
         * @param renderTarget Target to render objects on
         */
        explicit Drawer(IME::Graphics::Window &renderTarget);

        /**
         * @brief Draw objects on a render target
         * @param entities Objects to be drawn
         */
        void drawEntities(const std::vector<std::shared_ptr<IME::Entity>>& entities);

    private:
        IME::Graphics::Window& renderTarget_;
    };
}

#endif
