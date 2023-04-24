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

#include <Mighter2d/core/grid/Grid.h>
#include <Mighter2d/core/object/GridObject.h>

namespace spm {
    class GameplayScene;

    /**
     * @brief Contains all game objects in the game
     */
    class Grid {
    public:
        /**
         * @brief Constructor
         * @param scene The gameplay scene
         */
        explicit Grid(GameplayScene& scene);

        /**
         * @brief Create the grid
         * @param Current level
         */
        void create(int level);

        /**
         * @brief Add an actor to the grid
         * @param object The actor to be added to the grid
         * @param index The index of the cell to add the actor to
         */
        void addGameObject(mighter2d::GridObject* object, mighter2d::Index index);

        /**
         * @brief Move an object to a different cell
         * @param object The object to be change tiles
         * @param index The index of the cell to move the object to
         */
        void changeObjectTile(mighter2d::GridObject* object, mighter2d::Index index);

        /**
         * @brief Apply a callback to each cell of the grid
         * @param callback The function to be applied
         */
        void forEachCell(const mighter2d::Callback<const mighter2d::Tile&>& callback);

        /**
         * @brief Flash the grid
         * @param level The current game level
         */
        void startFlash(int level);

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
        void onFlashStop(mighter2d::Callback<> callback);

        /**
         * @brief Check if the grid startFlash animation is playing or not
         * @return True if animation is currently playing, otherwise false
         */
        bool isFlashing();

        /**
         * @brief Get the scene the grid is in
         * @return The scene the grid is in
         */
        mighter2d::Scene& getScene();

    private:
        /**
         * @brief Set the grid background
         * @param level The current game level
         *
         * Level 0 = intro grid and level 1 to ... n = gameplay grid
         */
        void setBackground(int level);

    private:
        mighter2d::Grid grid_;
        mighter2d::SpriteSheet spriteSheet_;
        mighter2d::Sprite background_;
    };
}


#endif //SUPERPACMAN_GRID_H
