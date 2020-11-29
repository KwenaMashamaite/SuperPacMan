/**
 * @brief Frontend for the intro state
 */

#ifndef INTROVIEW_H
#define INTROVIEW_H

#include "ScoreView.h"

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
         * @brief Pass an event to be handled by the view
         * @param event Event to be handled
         */
        void handleEvent(sf::Event event);

    private:
        ScoreView scoreView_;
        IME::Graphics::UI::GuiContainer guiContainer_;
        IME::Vector2u windowSize_;
    };
}

#endif
