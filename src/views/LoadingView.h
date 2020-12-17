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

#ifndef LOADINGVIEW_H
#define LOADINGVIEW_H

#include <IME/graphics/Sprite.h>
#include <IME/graphics/ui/GuiContainer.h>

namespace SuperPacMan {
    /**
     * @brief Frontend for the loading state
     */
    class LoadingView {
    public:
        /**
         * @brief Create the view
         * @param renderTargetSize Size of the window the view is being
         *        rendered on
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
         * @brief Create the bar to show the progress of the resource
         *        loading
         */
        void createProgressBar();

    private:
        IME::Vector2u windowSize_;           //!< Game window Size
        IME::Graphics::Sprite engineLogo_;   //!< Third party engine logo
        IME::Graphics::Sprite mascot_;       //!< Super Pac-Man mascot
        IME::UI::GuiContainer guiContainer_; //!< Container for all widgets in this view
    };
}

#endif
