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

#ifndef SUPERPACMAN_PACMAN_H
#define SUPERPACMAN_PACMAN_H

#include <IME/core/game_object/GameObject.h>

namespace spm {
    /**
     * @brief Player controlled actor
     */
    class PacMan : public ime::GameObject {
    public:
        /**
         * @brief Constructor
         * @param scene The scene the actor belongs to
         */
        explicit PacMan(ime::Scene& scene);

        /**
         * @brief Set the number of lives
         * @param numOfLives New number of lives
         */
        void setLivesCount(unsigned int numOfLives);

        /**
         * @brief Get number of lives remaining
         * @return The number of lives remaining
         */
        unsigned int getLivesCount() const;

        /**
         * @brief Get the name of this class
         * @return The name of this class
         */
        std::string getClassName() const override;

    private:
        unsigned int livesCount_; //!< The actors current number of lives
    };
}

#endif
