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

#include "Utils.h"
#include "../entities/AllEntities.h"
#include "../common/Constants.h"
#include "../entities/states/pacman/PacManIdleState.h"
#include "../entities/states/ghost/GhostIdleState.h"
#include "../entities/states/ghost/FrightenedState.h"
#include "../entities/states/ghost/ScatterState.h"
#include "../entities/states/ghost/ChaseState.h"
#include "../entities/states/pacman/SuperState.h"
#include "../entities/states/pacman/NormalState.h"
#include "../entities/states/ghost/EatenState.h"

/**
 * @brief Get the key id that will open a door at a specific index
 * @param doorIndex Index of the door
 * @return The keys id
 *
 * The door is always opened by a key with an id that matches the doors
 * locker. As such, Key-locker combinations are hardcoded, this means that
 * a certain key will always open specific doors, regardless of where the
 * key it is placed
 */
int getLockerId(ime::Index doorIndex);

namespace pacman::Utils {
    ObjectsContainer createObjects(ime::TileMap &grid) {
        int keyId = 0;
        ObjectsContainer objects;
        grid.forEachTile([&](ime::Tile& tile) {
            if (tile.getId() == 'D') { //Doors
                auto door = std::make_shared<Door>(grid.getTileSize());
                if (tile.getIndex().row % 2 == 0)
                    door->setOrientation(Orientation::Horizontal);
                grid.addChild(door, tile.getIndex());
                objects["doors"].push_back(std::move(door));
            } else if (tile.getId() == '#' || tile.getId() == '|') { //Walls
                auto wall = std::make_shared<Wall>(grid.getTileSize());
                grid.addChild(wall, tile.getIndex());
                objects["walls"].push_back(std::move(wall));
            } else if (tile.getId() == 'K') { //Keys
                auto key = std::make_shared<Key>(grid.getTileSize(), keyId++);
                grid.addChild(key, tile.getIndex());
                objects["keys"].push_back(std::move(key));
            } else if (tile.getId() == 'F') { //Fruits
                auto fruit =  std::make_shared<Fruit>(grid.getTileSize(), "apple");
                grid.addChild(fruit, tile.getIndex());
                objects["fruits"].push_back(std::move(fruit));
            } else if (tile.getId() == 'E' || tile.getId() == 'S') { //Power pellet or super pellet
                auto pelletType = PelletType::SuperPellet;
                if (tile.getId() == 'E')
                    pelletType = PelletType::PowerPellet;
                auto pellet = std::make_shared<Pellet>(pelletType, grid.getTileSize());
                grid.addChild(pellet, tile.getIndex());
                objects["pellets"].push_back(std::move(pellet));
            } else if (tile.getId() == 'X') { //PacMan
                auto pacman = std::make_shared<PacMan>(grid.getTileSize());
                pacman->setSpeed(Constants::PacManNormalSpeed);
                pacman->pushState(PacMan::States::Idle, std::make_shared<PacManIdleState>(pacman));
                grid.addChild(pacman, tile.getIndex());
                objects["pacman"].push_back(std::move(pacman));
            } else if (tile.getId() == 'B' || tile.getId() == 'P' || tile.getId() == 'C' || tile.getId() == 'I') {
                auto ghostName = Ghost::Name::Blinky;
                if (tile.getId() == 'P')
                    ghostName = Ghost::Name::Pinky;
                else if (tile.getId() == 'C')
                    ghostName = Ghost::Name::Clyde;
                else if (tile.getId() == 'I')
                    ghostName = Ghost::Name::Inky;

                auto ghost = std::make_shared<Ghost>(ghostName, grid.getTileSize());
                ghost->pushState(Ghost::States::Idle, std::make_shared<GhostIdleState>(ghost));
                ghost->setVulnerable(false);
                grid.addChild(ghost, tile.getIndex());
                objects["ghosts"].push_back(std::move(ghost));
            }
        });
        return objects;
    }

    void removeInactiveObjectsFromContainer(std::vector<std::shared_ptr<ime::Entity>>& entities) {
        entities.erase(std::remove_if(entities.begin(), entities.end(), [](std::shared_ptr<ime::Entity>& entity) {
            return !entity->isActive();
        }), entities.end());
    }

    std::string convertToString(ime::Direction direction) {
        switch (direction) {
            case ime::Direction::Left:
                return "Left";
            case ime::Direction::Right:
                return "Right";
            case ime::Direction::Up:
                return "Up";
            case ime::Direction::Down:
                return "Down";
            default:
                return "Unknown";
        }
    }

    void frightenGhost(std::shared_ptr<ime::Entity> ghost, ime::TileMap &grid) {
        auto frightenedState = std::make_shared<FrightenedState>(ghost, grid);
        frightenedState->setTimeout(10.0f, [ghost, &grid] {
            Utils::scatterGhost(ghost, grid);
        });
        std::dynamic_pointer_cast<Ghost>(ghost)->pushState(Ghost::States::Frightened, std::move(frightenedState));
    }

    void scatterGhost(std::shared_ptr<ime::Entity> ghost, ime::TileMap &grid) {
        auto scatterPos = ScatterPosition::TopRightCorner;
        switch (std::dynamic_pointer_cast<Ghost>(ghost)->getGhostName()) {
            case Ghost::Name::Blinky:
                break;
            case Ghost::Name::Pinky:
                scatterPos = ScatterPosition::TopLeftCorner;
                break;
            case Ghost::Name::Inky:
                scatterPos = ScatterPosition::BottomRightCorner;
                break;
            case Ghost::Name::Clyde:
                scatterPos = ScatterPosition::BottomLeftCorner;
                break;
        }
        auto scatterState = std::make_shared<ScatterState>(scatterPos, ghost, grid);
        scatterState->setTimeout(10.0f, [ghost, &grid] {
            std::dynamic_pointer_cast<Ghost>(ghost)->pushState(Ghost::States::Eaten,
                std::make_shared<EatenState>(ghost, grid));
        });
        std::dynamic_pointer_cast<Ghost>(ghost)->pushState(Ghost::States::Scatter, std::move(scatterState));
    }

    bool unlockDoor(std::shared_ptr<ime::Entity> doorPtr, std::shared_ptr<ime::Entity> key) {
        auto door = std::dynamic_pointer_cast<Door>(doorPtr);
        if (!door->isLocked())
            return false;
        door->unlockWith(*std::dynamic_pointer_cast<Key>(key));
        if (!door->isLocked())
            return true;
        return false;
    }

    void lockAllDoors(ime::TileMap& grid) {
        grid.forEachTileWithId('D', [&grid](ime::Tile& tile) {
            auto door = std::dynamic_pointer_cast<Door>(grid.getOccupant(tile));
            door->addDoorLocker(std::make_unique<DoorLocker>(getLockerId(tile.getIndex())));
            door->lockWith(Key({}, getLockerId(tile.getIndex())));
        });
    }

    void enlargePacman(std::shared_ptr<ime::Entity> pacman, float duration) {
        auto superState = std::make_shared<SuperState>(pacman);
        superState->setTimeout(duration, [pacman] {
            std::dynamic_pointer_cast<PacMan>(pacman)->pushState(PacMan::States::Normal,
                 std::make_shared<NormalState>(pacman));
        });
        std::dynamic_pointer_cast<PacMan>(pacman)->pushState(PacMan::States::Super, std::move(superState));
    }

    void flashGrid(ime::TileMap& grid, int level) {
        if (level >= 1 && level <= 4)
            grid.getBackground().switchAnimation("flash-blue");
        else if (level >= 5 && level <= 8)
            grid.getBackground().switchAnimation("flash-orange");
        else if (level >= 9 && level <= 12)
            grid.getBackground().switchAnimation("flash-purple");
        else if (level >= 13 && level <= 16)
            grid.getBackground().switchAnimation("flash-pink");
        else
            grid.getBackground().switchAnimation("flash-green");
    }

    void chasePacMan(std::shared_ptr<ime::Entity> pacman,
         std::shared_ptr<ime::Entity> ghost, ime::TileMap &grid)
    {
        auto chaseState = std::make_shared<ChaseState>(ghost, grid, grid.getTileOccupiedByChild(pacman));
        chaseState->setTimeout(7.0f, [&grid, ghost] {
            Utils::scatterGhost(ghost, grid);
        });
        std::dynamic_pointer_cast<Ghost>(ghost)->pushState(Ghost::States::Chasing, std::move(chaseState));
    }

    void triggerAnimationSwitch(std::shared_ptr<ime::Entity> entity) {
        if (entity) {
            auto previousDirection = entity->getDirection();
            entity->setDirection(ime::Direction::Unknown);
            entity->setDirection(previousDirection);
        }
    }
}

int getLockerId(ime::Index doorIndex) {
    if (doorIndex == ime::Index{4, 4} || doorIndex == ime::Index{6, 4})
        return 0;
    else if (doorIndex == ime::Index{4, 20} || doorIndex == ime::Index{6, 20})
        return 1;
    else if (doorIndex == ime::Index{5, 7} || doorIndex == ime::Index{4, 12})
        return 2;
    else if (doorIndex == ime::Index{5, 17})
        return 3;
    else if (doorIndex == ime::Index{10, 4} || doorIndex == ime::Index{13, 3} || doorIndex == ime::Index{13, 5})
        return 4;
    else if (doorIndex == ime::Index{10, 20} || doorIndex == ime::Index{13, 19} || doorIndex == ime::Index{13, 21})
        return 5;
    else if (doorIndex == ime::Index{16, 4} || doorIndex == ime::Index{20, 4})
        return 6;
    else if (doorIndex == ime::Index{13, 7} || doorIndex == ime::Index{16, 8} || doorIndex == ime::Index{17, 11})
        return 7;
    else if (doorIndex == ime::Index{13, 17} || doorIndex == ime::Index{17, 13} || doorIndex == ime::Index{16, 16})
        return 8;
    else if (doorIndex == ime::Index{16, 20} || doorIndex == ime::Index{20, 20})
        return 9;
    else if (doorIndex == ime::Index{22, 12} || doorIndex == ime::Index{13, 1} || doorIndex == ime::Index{13, 23})
        return 10;
    else if (doorIndex == ime::Index{22, 8} || doorIndex == ime::Index{25, 7} || doorIndex == ime::Index{24, 12})
        return 11;
    else if (doorIndex == ime::Index{22, 16} || doorIndex == ime::Index{25, 17})
        return 12;
    else if (doorIndex == ime::Index{22, 4} || doorIndex == ime::Index{24, 4})
        return 13;
    else if (doorIndex == ime::Index{22, 20} || doorIndex == ime::Index{24, 20})
        return 14;
    else return -1;
}
