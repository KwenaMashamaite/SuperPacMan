/**
 * @brief Frontend for the main menu state
 */

#ifndef MAINMENUVIEW_H
#define MAINMENUVIEW_H

#include <IME/graphics/Sprite.h>
#include <IME/graphics/ui/GuiContainer.h>
#include "ScoreView.h"

namespace SuperPacMan {
    /**
     * @brief States the menu can be in
     */
    enum class SubView {
        Main,
        Info
    };

    class MainMenuView {
    public:
        explicit MainMenuView(IME::Graphics::Window& renderTarget);
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
         * @brief Switch to a different view within this view
         * @param view View to switch to
         */
        void setSubView(SubView view);

        /**
         * @brief Get access to a widget in the view
         * @param widgetName Name of the widget to get access to
         * @return The specified widget if it exists, otherwise a nullptr
         *
         * The object will be casted to the desired type if it's valid
         */
        template <class T>
        std::shared_ptr<T> getWidget(const std::string& widgetName) const {
            auto widget = mainViewContainer_.getWidget<T>(widgetName);
            if (widget)
                return widget;
            else
                return infoViewContainer_.getWidget<T>(widgetName);
        }

    private:
        /**
         * @brief Create the pacman logo
         */
        void createLogo();

        /**
         * @brief Create the menu navigation buttons
         */
        void createButtons();

    private:
        //
        ScoreView scoreView_;
        //Container for main view widgets
        IME::Graphics::UI::GuiContainer mainViewContainer_;
        //Container for info view widgets
        IME::Graphics::UI::GuiContainer infoViewContainer_;
        //The games logo
        IME::Graphics::Sprite pacmanLogo_;
        //The current view of the main menu
        SubView currentView_;
        //Size of the render target for this view
        IME::Vector2u windowSize_;
    };
}

#endif
