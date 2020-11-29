/**
 * @brief
 */

#ifndef SCOREVIEW_H
#define SCOREVIEW_H

#include <IME/graphics/Window.h>
#include <IME/graphics/ui/GuiContainer.h>

namespace SuperPacMan {
    class ScoreView {
    public:
        /**
         * @brief Create view
         * @param renderTarget Target to render view on
         */
        explicit ScoreView(IME::Graphics::Window& renderTarget);

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
        //Container for all widgets
        IME::Graphics::UI::GuiContainer guiContainer_;
        //Size of the render target
        IME::Vector2u windowSize_;
    };
}

#endif
