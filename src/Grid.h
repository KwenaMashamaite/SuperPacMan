////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
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

#ifndef SUPERPACMAN_GRID_H
#define SUPERPACMAN_GRID_H

#include <IME/core/grid/Grid2D.h>
#include <IME/core/object/GridObject.h>

namespace spm {
    /**
     * @brief Contains all game objects in the game
     */
    class Grid {
    public:
        /**
         * @brief Constructor
         * @param grid Third party grid
         */
        explicit Grid(ime::Grid2D& grid);

        /**
         * @brief Create the grid
         * @param Current level
         */
        void create(int level);

        /**
         * @brief Initialize the grid
         */
        void init();

        /**
         * @brief Add an actor to the grid
         * @param object The actor to be added to the grid
         * @param index The index of the cell to add the actor to
         */
        void addGameObject(ime::GridObject::Ptr object, ime::Index index);

        /**
         * @brief Add an actor to the grid
         * @param object The actor to be added to the grid
         * @param index The index of the cell to add the actor to
         */
        void addGameObject(ime::GridObject* object, ime::Index index);

        /**
         * @brief Remove an game object from the grid
         * @param gameObject The game object to be removed
         */
        void removeGameObject(ime::GridObject *gameObject);

        /**
         * @brief Apply a callback to each cell of the grid
         * @param callback The function to be applied
         */
        void forEachCell(const ime::Callback<const ime::Tile&>& callback);

        /**
         * @brief Apply a callback to each actor in the grid
         * @param callback The function to be applied
         */
        void forEachGameObject(const ime::Callback<ime::GridObject*>& callback);

        /**
         * @brief Flash the grid
         * @param level The current game level
         *
         * This function will toggle the grid colour between its current
         * colour and a white colour for predetermined number of seconds
         */
        void flash(int level);

        /**
         * @brief Execute a callback when the grid animation finishes
         * @param callback The function to be executed
         *
         * @note Only one callback may be assigned to this event. A new
         * callback removes the previous callback.
         *
         * By default, there is no callback assigned to this event.
         * To remove the callback pass a nullptr
         */
        void onFlashStop(ime::Callback<> callback);

        /**
         * @brief Check if the grid flash animation is playing or not
         * @return True if animation is currently playing, otherwise false
         */
        bool isFlashing();

        /**
         * @brief Update the grid
         * @param deltaTime Time passed since ;ast scene update
         */
        void update(ime::Time deltaTime);

        /**
         * @brief Get the scene the grid is in
         * @return The scene the grid is in
         */
        ime::Scene& getScene();

        /**
         * @brief Implicit conversion operator
         */
        operator ime::Grid2D& ();

    private:
        /**
         * @brief Set the grid background
         * @param level The current game level
         *
         * Level 0 = intro grid and level 1 to ... n = gameplay grid
         */
        void setBackground(int level);

    private:
        ime::Grid2D& grid_;
        ime::SpriteSheet spriteSheet_;
        ime::Sprite background_;
    };
}


#endif //SUPERPACMAN_GRID_H
