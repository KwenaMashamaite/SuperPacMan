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

#ifndef SUPERPACMAN_MAINMENUVIEW_H
#define SUPERPACMAN_MAINMENUVIEW_H

#include <IME/graphics/Sprite.h>
#include <IME/ui/GuiContainer.h>
#include "CommonView.h"

namespace pacman {
    /**
     * @brief Main menu sub views
     */
    enum class SubView {
        MainMenu,    //!< View rendered when main menu state is rendered
        OptionsMenu, //!< View rendered when options view is clicked
        CreditsMenu, //!< View rendered when credits button is clicked
    };

    /**
     * @brief Frontend for the main menu state
     */
    class MainMenuView {
    public:
        /**
         * @brief Construct view
         * @param window Window to render view on
         */
        explicit MainMenuView(ime::Window& window);

        /**
         * @brief Initialize the view
         */
        void init();

        /**
         * @brief Render the view
         */
        void render();

        /**
         * @brief Pass an event to be handled by the current main menu view
         * @param event Event to be handled
         */
        void handleEvent(ime::Event event);

        /**
         * @brief Switch to a different view within the main menu
         * @param view View to switch to
         */
        void setSubView(SubView view);

        /**
         * @brief Get the view that is currently rendered
         * @return Current main menu view
         */
        SubView getView() const;

        /**
         * @brief Get access to a widget in the view
         * @param widgetName Name of the widget to get access to
         * @return The requested widget if it exists, otherwise a nullptr
         *
         * The widget will be casted to the desired type if it's valid
         *
         * Example of getting a button widget called "play_btn"
         * @code
         * view.getWidget<ime::ui::Button>("play_btn");
         * @endcode
         */
        template <class T>
        std::shared_ptr<T> getWidget(const std::string& widgetName) const {
            return guiContainer_.getWidget<T>(widgetName);
        }

    private:
        /**
         * @brief Create the main view
         *
         * This view is rendered when the main menu state is entered and also
         * when a subview returns (usually when back button is clicked)
         */
        void createMainMenuView();

        /**
         * @brief Create the view that is rendered when the options button
         *        is clicked
         */
        void createOptionsMenuView();

        /**
         * @brief Create the view that is rendered when the credits button is
         *        clicked
         */
        void createCreditsMenuView();

    private:
        ime::ui::GuiContainer guiContainer_;    //!< Container for all main menu widgets
        ime::Vector2u windowSize_;              //!< Size of the render target
        SubView subView_;                       //!< View that is currently rendered
    };
}

#endif
