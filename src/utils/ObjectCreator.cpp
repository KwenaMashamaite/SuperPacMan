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

#include "ObjectCreator.h"
#include "GameObjects/GameObjects.h"
#include "Animations/FruitAnimation.h"

namespace spm {
    namespace {
        ///////////////////////////////////////////////////////////////
        ime::GameObject::Ptr createDoor(const ime::Tile& tile, ime::Scene& scene) {
            auto door = std::make_unique<Door>(scene);

            if (tile.getIndex().row % 2 == 0)
                door->setOrientation(Door::Orientation::Horizontal);
            else
                door->setOrientation(Door::Orientation::Vertical);

            return door;
        }
    } // namespace anonymous

    ///////////////////////////////////////////////////////////////
    void ObjectCreator::createObjects(Grid &grid) {
        grid.forEachCell([&grid, slowDownSensorCount = 0](const ime::Tile& tile) mutable {
            ime::GameObject::Ptr gameObject;

            if (tile.getId() == 'X') {
                gameObject = std::make_unique<PacMan>(grid.getScene());
            } else if (tile.getId() == 'T' || tile.getId() == 'H' || tile.getId() == '!' || tile.getId() == '+') { // Sensors
                gameObject = std::make_unique<Sensor>(grid.getScene());

                if (tile.getId() == 'T')
                    gameObject->setTag("teleportationSensor");
                if (tile.getId() == 'H' || tile.getId() == '+') {
                    gameObject->setTag("slowDownSensor" + std::to_string(++slowDownSensorCount));

                    if (tile.getId() == '+') { // Sensor + Door,
                        grid.addGameObject(std::move(gameObject), tile.getIndex());
                        grid.addGameObject(createDoor(tile, grid.getScene()), tile.getIndex());
                        return;
                    }
                }
            } else if (tile.getId() == 'K')
                gameObject = std::make_unique<Key>(grid.getScene());
            else if (tile.getId() == 'F')
                gameObject = std::make_unique<Fruit>(grid.getScene());
            else if (tile.getId() == 'E')
                gameObject = std::make_unique<Pellet>(grid.getScene(), Pellet::Type::Power);
            else if (tile.getId() == 'S')
                gameObject = std::make_unique<Pellet>(grid.getScene(), Pellet::Type::Super);
            else if (tile.getId() == 'D')
                gameObject = createDoor(tile, grid.getScene());
            else if (tile.getId() == '#' || tile.getId() == '|' || tile.getId() == 'N') {// Walls
                gameObject = std::make_unique<Wall>(grid.getScene());

                // Hidden wall that Only pacman can pass through
                if (tile.getId() == 'N')
                    gameObject->setCollisionGroup("hiddenWall");
            } else if (tile.getId() == '?') {
                gameObject = ime::GameObject::create(grid.getScene());
                ime::Animation::Ptr fruitSlideAnim = FruitAnimation().getAnimation();

                if (tile.getIndex().colm == 11) {
                    gameObject->setTag("leftBonusFruit");
                    fruitSlideAnim->setLoop(false);
                    fruitSlideAnim->setTimescale(2.0f);
                } else
                    gameObject->setTag("rightBonusFruit");

                gameObject->getSprite().getAnimator().addAnimation(std::move(fruitSlideAnim));
                gameObject->getSprite().setScale(2.0f, 2.0f);
                gameObject->getSprite().setOrigin(8, 8);
            }else {
                if (tile.getId() == 'B')
                    gameObject = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Red);
                else if (tile.getId() == 'P')
                    gameObject = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Pink);
                else if (tile.getId() == 'I')
                    gameObject = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Cyan);
                else if (tile.getId() == 'C')
                    gameObject = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Orange);
                else
                    return;
            }

            grid.addGameObject(std::move(gameObject), tile.getIndex());
        });
    }

} // namespace spm
