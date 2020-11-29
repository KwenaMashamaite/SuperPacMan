/**
 * @brief Frontend for the intro state
 */

#ifndef INTROVIEW_H
#define INTROVIEW_H

#include "CommonView.h"

namespace SuperPacMan {
    class IntroView {
    public:
        /**
         * @brief Construct the view
         * @param renderTarget Target to render view on
         */
        explicit IntroView(IME::Graphics::Window& renderTarget);

        /**
         * @brief Initialize the view
         */
        void init();

        /**
         * @brief Render the view
         * @param renderTarget Target to render view on
         */
        void render(IME::Graphics::Window& renderTarget);

        /**
         * @brief Update view
         * @param deltaTime Time passed since view was last updated
         */
        void update(float deltaTime);

    private:
        CommonView commonView_;
        IME::Graphics::UI::GuiContainer guiContainer_;
        IME::Vector2u windowSize_;
    };
}

#endif
