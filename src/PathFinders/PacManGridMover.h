////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#ifndef SUPERPACMAN_PACMANGRIDMOVER_H
#define SUPERPACMAN_PACMANGRIDMOVER_H

#include <IME/core/physics/grid/KeyboardGridMover.h>

namespace spm {
    class PacMan;

    /**
     * @brief Controls the PathFinders of pacman in the grid using the keyboard
     */
    class PacManGridMover : public ime::KeyboardGridMover {
    public:
        /**
         * @brief Constructor
         * @param grid The grid pacman is to be moved in
         * @param pacman Pointer to a PacMan instance
         *
         * @warning @a pacman must not be a nullptr
         */
        PacManGridMover(ime::Grid2D& grid, PacMan* pacman);

        /**
         * @brief Initialize the grid mover
         */
        void init();

        /**
         * @brief Destructor
         */
        ~PacManGridMover() override;

    private:
        int pacmanStateChangeId_;         //!< State chang handler id
        ime::Direction pendingDirection_; //!< Direction pacman wishes to go in without
    };
}

#endif //PACMAN_PACMANGRIDMOVER_H
