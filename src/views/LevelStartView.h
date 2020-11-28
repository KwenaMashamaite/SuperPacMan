/**
 * @brief View displayed before a level is started or restarted
 */

#ifndef LEVELSTARTVIEW_H
#define LEVELSTARTVIEW_H

#include <IME/graphics/Sprite.h>
#include "ScoreView.h"

namespace SuperPacMan {
    class LevelStartView {
    public:
        /**
         * @brief Create level start view
         * @param renderTarget Target view is rendered on
         * @param level Current level
         * @param score Current score
         * @param highscore Current highscore
         */
        LevelStartView(IME::Graphics::Window& renderTarget, unsigned int level, int score, int highscore);

        /**
         * @brief Initialize the view
         */
        void init();

        /**
         * @brief Render the view
         * @param renderTarget Target to render view on
         */
        void render(IME::Graphics::Window& renderTarget);

    private:
        //Container
        IME::Graphics::UI::GuiContainer guiContainer_;
        //Size of the render target
        IME::Vector2u windowSize_;
        //View to show the scores
        ScoreView scoreView_;
        //Current level fruit
        IME::Graphics::Sprite fruit_;
        //Current level
        unsigned int level_;
        //current score
        int score_;
        //current highscore
        int highscore_;
    };
}

#endif
