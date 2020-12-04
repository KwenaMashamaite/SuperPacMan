/**
 * @brief Frontend for the loading state
 */

#ifndef LOADINGVIEW_H
#define LOADINGVIEW_H

#include <IME/graphics/Sprite.h>
#include <IME/graphics/ui/GuiContainer.h>

namespace SuperPacMan {
    class LoadingView {
    public:
        /**
         * @brief Create the view
         * @param renderTargetSize Size of the window the view is being rendered
         *          on
         */
        explicit LoadingView(IME::Vector2u renderTargetSize);

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

        /**
         * @brief Get access to a widget in the view
         * @param widgetName Name of the widget to get access to
         * @return The specified widget if it exists, otherwise a nullptr
         *
         * The object will be casted to the desired type if it's valid
         */
        template <class T>
        std::shared_ptr<T> getWidget(const std::string& widgetName) const {
            return guiContainer_.getWidget<T>(widgetName);
        }

    private:
        /**
         * @brief Create the title of the game
         */
        void createTitle();

        /**
         * @brief Create the bar to show the progress of the resource loading
         */
        void createProgressBar();

    private:
        //Window Size
        IME::Vector2u windowSize_;
        //Third party engine logo
        IME::Graphics::Sprite engineLogo_;
        //Games mascot
        IME::Graphics::Sprite mascot_;
        //Container for all widgets
        IME::UI::GuiContainer guiContainer_;
    };
}

#endif
