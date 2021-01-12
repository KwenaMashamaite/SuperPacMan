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

#ifndef SUPERPACMAN_STARTUPVIEW_H
#define SUPERPACMAN_STARTUPVIEW_H

#include <IME/graphics/Window.h>
#include <IME/ui/GuiContainer.h>

namespace pacman {
    /**
     * @brief Frontend for the start up state
     *
     * This view is displayed when the game is opened i.e the first graphic
     * the user sees
     */
    class StartUpView {
    public:
        /**
         * @brief Constructor
         * @param window Window to render view on
         */
        explicit StartUpView(ime::Window& window);

        /**
         * @brief Initialize the view
         */
        void init();

        /**
         * @brief Render the view
         */
        void render();

        /**
         * @brief Get access to a widget in the view
         * @param widgetName Name of the widget to get access to
         * @return The required widget if it exists, otherwise a nullptr
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
        ime::ui::GuiContainer guiContainer_; //!< Container for all gui widgets
    };
}

#endif
