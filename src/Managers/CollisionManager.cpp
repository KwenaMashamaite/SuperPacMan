#include "CollisionManager.h"
#include "GameObjects/PacMan.h"
#include "GameObjects/Ghost.h"

namespace spm {
    void CollisionManager::init(GameplayScene &gameplayScene) {
        auto* pacman = gameplayScene.gameObjectsManager_.getPacMan();
        /*collisionResponseRegisterer_.registerCollisionWithFruit(pacman);
        collisionResponseRegisterer_.registerCollisionWithKey(pacman);
        collisionResponseRegisterer_.registerCollisionWithDoor(pacman);
        collisionResponseRegisterer_.registerCollisionWithPowerPellet(pacman);
        collisionResponseRegisterer_.registerCollisionWithSuperPellet(pacman);
        collisionResponseRegisterer_.registerCollisionWithGhost(pacman);
        collisionResponseRegisterer_.registerCollisionWithStar(pacman);
        collisionResponseRegisterer_.registerCollisionWithTeleportationSensor(pacman);

        gameplayScene.getGameObjects().forEachInGroup("Ghost", [this] (ime::GameObject* ghostBase){
            auto* ghost = static_cast<Ghost*>(ghostBase);
            collisionResponseRegisterer_.registerCollisionWithPacMan(ghost);
            collisionResponseRegisterer_.registerCollisionWithTeleportationSensor(ghost);
            collisionResponseRegisterer_.registerCollisionWithSlowDownSensor(ghost);
        });*/
    }

    // Collision Resolver class

    void CollisionManager::CollisionResolver::resolveFruitCollision(ime::GridObject *fruit) {

    }

    void CollisionManager::CollisionResolver::resolveKeyCollision(ime::GridObject *key) {

    }

    void CollisionManager::CollisionResolver::resolvePowerPelletCollision(ime::GridObject *pellet) {

    }

    void CollisionManager::CollisionResolver::resolveSuperPelletCollision(ime::GridObject *pellet) {

    }

    void CollisionManager::CollisionResolver::resolvePacmanCollision(ime::GridObject *pacman, ime::GridObject *otherGameObject) {

    }

    void CollisionManager::CollisionResolver::resolveGhostCollision(ime::GridObject *ghost, ime::GridObject *otherGameObject) {

    }

    void CollisionManager::CollisionResolver::resolveStarCollision(ime::GridObject *star, ime::GridObject *otherGameObject) {

    }

    void CollisionManager::CollisionResolver::resolveDoorCollision(ime::GridObject *door, ime::GridObject *otherGameObject) {

    }

    void CollisionManager::CollisionResolver::resolveSlowDownSensorCollision(ime::GridObject *sensor, ime::GridObject *objectOnSensor) {

    }
}