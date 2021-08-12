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

#include "src/utils/ObjectCreator.h"
#include "src/models/actors/Actors.h"
#include <IME/core/physics/rigid_body/colliders/BoxCollider.h>

namespace spm {
    namespace {
        ///////////////////////////////////////////////////////////////
        ime::GameObject::Ptr createDoor(const ime::Tile& tile, ime::Scene& scene) {
            auto door = std::make_unique<Door>(scene);
            door->getUserData().addProperty({"gridIndex", tile.getIndex()});
            door->getUserData().addProperty(ime::Property{"scene", std::ref(scene)});

            if (tile.getIndex().row % 2 == 0)
                door->setOrientation(Orientation::Horizontal);
            else
                door->setOrientation(Orientation::Vertical);

            return door;
        }
    } // namespace anonymous

    ///////////////////////////////////////////////////////////////
    void ObjectCreator::createObjects(Grid &grid) {
        grid.forEachCell([&grid, keyId = 0](const ime::Tile& tile) mutable {
            ime::GameObject::Ptr actor;

            if (tile.getId() == 'X') {
                actor = std::make_unique<PacMan>(grid.getScene());
            } else if (tile.getId() == 'T' || tile.getId() == 'H' || tile.getId() == '!' || tile.getId() == '+') { // Sensors
                actor = std::make_unique<Sensor>(grid.getScene());

                if (tile.getId() == 'H' || tile.getId() == '+') {
                    actor->setTag("slowLaneEntrySensor");
                    actor->setCollisionGroup("slowLaneEntrySensor");

                    if (tile.getId() == '+') { // Sensor + Door,
                        grid.addActor(std::move(actor), tile.getIndex());
                        grid.addActor(createDoor(tile, grid.getScene()), tile.getIndex());
                        return;
                    }

                } else if (tile.getId() == '!') {
                    actor->setTag("slowLaneExitSensor");
                    actor->setCollisionGroup("slowLaneExitSensor");
                }

            } else if (tile.getId() == 'K')
                actor = std::make_unique<Key>(grid.getScene(), keyId++);
            else if (tile.getId() == 'F')
                actor = std::make_unique<Fruit>(grid.getScene());
            else if (tile.getId() == 'E')
                actor = std::make_unique<Pellet>(grid.getScene(), Pellet::Type::Power);
            else if (tile.getId() == 'S')
                actor = std::make_unique<Pellet>(grid.getScene(), Pellet::Type::Super);
            else if (tile.getId() == 'D')
                actor = createDoor(tile, grid.getScene());
            else if (tile.getId() == '#' || tile.getId() == '|' || tile.getId() == 'N') {// Walls
                actor = std::make_unique<Wall>(grid.getScene());

                // Hidden wall that Only pacman can pass through
                if (tile.getId() == 'N')
                    actor->setCollisionGroup("hiddenWall");
            } else {
                if (tile.getId() == 'B')
                    actor = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Red);
                else if (tile.getId() == 'P')
                    actor = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Pink);
                else if (tile.getId() == 'I')
                    actor = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Cyan);
                else if (tile.getId() == 'C')
                    actor = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Orange);
                else
                    return;
            }

            actor->getUserData().addProperty({"scene", std::ref(grid.getScene())});
            grid.addActor(std::move(actor), tile.getIndex());
        });
    }

} // namespace spm
