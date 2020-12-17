////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#ifndef MAINMENUVIEW_H
#define MAINMENUVIEW_H

#include <IME/graphics/Sprite.h>
#include <IME/graphics/ui/GuiContainer.h>
#include "CommonView.h"

namespace SuperPacMan {
    /**
     * @brief Main menu sub views
     */
    enum class SubView {
        Main,   //!< Navigation view
        Info    //!< Information view
    };

    /**
     * @brief Frontend for the main menu state
     */
    class MainMenuView {
    public:
        /**
         * @brief Construct view
         * @param renderTarget Target to render view on
         */
        explicit MainMenuView(IME::Graphics::Window& renderTarget);

        /**
         * @brief Initialize the view
         * @param highScore Highest score from previous gameplay
         */
        void init(int highScore);

        /**
         * @brief Render the view
         * @param renderTarget Target to render view on
         */
        void render(IME::Graphics::Window& renderTarget);

        /**
         * @brief Handle an event
         * @param event Event to be handled
         *
         * This function ensures that mouse clicks and mose move
         * events are captured by the view
         */
        void handleEvent(sf::Event event);

        /**
         * @brief Update view
         * @param deltaTime Time passed since view was last updated
         */
        void update(float deltaTime);

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
         * @brief Create the navigation buttons
         */
        void createButtons();

    private:
        CommonView commonView_;                   //!< View displayed by all states
        IME::UI::GuiContainer mainViewContainer_; //!< Container for all main menu subview widgets
        IME::UI::GuiContainer infoViewContainer_; //!< Container for all info subview widgets
        IME::Graphics::Sprite pacmanLogo_;        //!< Super PacMan logo
        SubView currentView_;                     //!< View that is currently rendered
        IME::Vector2u windowSize_;                //!< Size of the games render target
    };
}

#endif
