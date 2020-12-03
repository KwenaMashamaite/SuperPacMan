#include "Utils.h"
#include "../entities/AllEntities.h"
#include "../common/Constants.h"
#include "../entities/states/pacman/PacManIdleState.h"
#include "../entities/states/ghost/GhostIdleState.h"

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
int getLockerId(IME::Index doorIndex);

using namespace IME::Graphics;

namespace SuperPacMan::Utils {
    ObjectsContainer createObjects(IME::TileMap &grid) {
        int keyId = 0;
        ObjectsContainer objects;
        grid.forEachTile([&](Tile& tile) {
            if (tile.getId() == 'D') { //Doors
                auto door = std::make_shared<Door>(grid.getTileSize());
                if (tile.getIndex().row % 2 == 0)
                    door->setOrientation(Orientation::Horizontal);
                door->addDoorLocker(std::make_unique<DoorLocker>(getLockerId(tile.getIndex())));
                door->lockWith(Key({}, getLockerId(tile.getIndex())));
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
                pacman->pushState(std::make_shared<PacManIdleState>());
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
                ghost->pushState(std::make_shared<GhostIdleState>());
                ghost->setVulnerable(false);
                grid.addChild(ghost, tile.getIndex());
                objects["ghosts"].push_back(std::move(ghost));
            }
        });
        return objects;
    }

    void removeDeadObjectsFromContainer(std::vector<std::shared_ptr<IME::Entity>>& entities) {
        entities.erase(std::remove_if(entities.begin(), entities.end(), [](std::shared_ptr<IME::Entity>& entity) {
            return !entity->isAlive();
        }), entities.end());
    }
}

int getLockerId(IME::Index doorIndex) {
    if (doorIndex == IME::Index{4, 3} || doorIndex == IME::Index{6, 3})
        return 0;
    else if (doorIndex == IME::Index{4, 19} || doorIndex == IME::Index{6, 19})
        return 1;
    else if (doorIndex == IME::Index{5, 6} || doorIndex == IME::Index{4, 11})
        return 2;
    else if (doorIndex == IME::Index{5, 16})
        return 3;
    else if (doorIndex == IME::Index{10, 3} || doorIndex == IME::Index{13, 2} || doorIndex == IME::Index{13, 4})
        return 4;
    else if (doorIndex == IME::Index{10, 19} || doorIndex == IME::Index{13, 18} || doorIndex == IME::Index{13, 20})
        return 5;
    else if (doorIndex == IME::Index{16, 3} || doorIndex == IME::Index{20, 3})
        return 6;
    else if (doorIndex == IME::Index{13, 6} || doorIndex == IME::Index{16, 7} || doorIndex == IME::Index{17, 10})
        return 7;
    else if (doorIndex == IME::Index{13, 16} || doorIndex == IME::Index{17, 12} || doorIndex == IME::Index{16, 15})
        return 8;
    else if (doorIndex == IME::Index{16, 19} || doorIndex == IME::Index{20, 19})
        return 9;
    else if (doorIndex == IME::Index{22, 11} || doorIndex == IME::Index{13, 0} || doorIndex == IME::Index{13, 22})
        return 10;
    else if (doorIndex == IME::Index{22, 7} || doorIndex == IME::Index{25, 6} || doorIndex == IME::Index{24, 11})
        return 11;
    else if (doorIndex == IME::Index{22, 15} || doorIndex == IME::Index{25, 16})
        return 12;
    else if (doorIndex == IME::Index{22, 3} || doorIndex == IME::Index{24, 3})
        return 13;
    else if (doorIndex == IME::Index{22, 19} || doorIndex == IME::Index{24, 19})
        return 14;
    else return -1;
}
