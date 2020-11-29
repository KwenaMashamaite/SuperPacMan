/**
 * @brief Defines the view that appears across all states
 *
 * This view includes the score and high score displayed at the top of
 * the screen, the flashing 1 UP text at the top left edge of the screen,
 * Pacman's current number of lives at the bottom left edge of the screen
 * the level fruits at the bottom right of the screen
 *
 * This view is used by all game states except the loading state
 */

#ifndef COMMONVIEW_H
#define COMMONVIEW_H

#include <IME/graphics/Window.h>
#include <IME/graphics/ui/GuiContainer.h>
#include <IME/graphics/Sprite.h>

namespace SuperPacMan {
    class CommonView {
    public:
        /**
         * @brief Create view
         * @param renderTarget Target to render view on
         * @param level Current level
         * @param lives Pacman;s remaining lives
         */
        CommonView(IME::Graphics::Window& renderTarget, int level, int lives);

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

        /**
         * @brief Get access to a widget in the view
         * @param widgetName Name of the widget to get access to
         * @return The specified widget if it exists, otherwise a nullptr
         *
         * The object will be casted to the desired type if it's valid
         */
        template <typename T>
        std::shared_ptr<T> getWidget(const std::string& widgetName) const {
            return guiContainer_.getWidget<T>(widgetName);
        }

    private:
        /**
         * @brief Create high score texts
         */
        void createText();

        /**
         * @brief Create fruit and pacman lives sprites
         */
        void createSprites();

    private:
        //Container for all widgets
        IME::Graphics::UI::GuiContainer guiContainer_;
        //Size of the render target
        IME::Vector2u windowSize_;
        //Stores all sprites
        std::vector<IME::Graphics::Sprite> sprites;
        //Current level
        int level_;
        //Remaining pacman lives
        int pacmanLives_;
        //How long the 1 up stays in hidden and shown states
        float flashTimeout_;
    };
}

#endif
