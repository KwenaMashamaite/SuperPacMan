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

        gameplayScene.getGameObjects().forEachInGroup("Ghost", [this] (mighter2d::GameObject* ghostBase){
            auto* ghost = static_cast<Ghost*>(ghostBase);
            collisionResponseRegisterer_.registerCollisionWithPacMan(ghost);
            collisionResponseRegisterer_.registerCollisionWithTeleportationSensor(ghost);
            collisionResponseRegisterer_.registerCollisionWithSlowDownSensor(ghost);
        });*/
    }

    // Collision Resolver class

    void CollisionManager::CollisionResolver::resolveFruitCollision(mighter2d::GridObject *fruit) {

    }

    void CollisionManager::CollisionResolver::resolveKeyCollision(mighter2d::GridObject *key) {

    }

    void CollisionManager::CollisionResolver::resolvePowerPelletCollision(mighter2d::GridObject *pellet) {

    }

    void CollisionManager::CollisionResolver::resolveSuperPelletCollision(mighter2d::GridObject *pellet) {

    }

    void CollisionManager::CollisionResolver::resolvePacmanCollision(mighter2d::GridObject *pacman, mighter2d::GridObject *otherGameObject) {

    }

    void CollisionManager::CollisionResolver::resolveGhostCollision(mighter2d::GridObject *ghost, mighter2d::GridObject *otherGameObject) {

    }

    void CollisionManager::CollisionResolver::resolveStarCollision(mighter2d::GridObject *star, mighter2d::GridObject *otherGameObject) {

    }

    void CollisionManager::CollisionResolver::resolveDoorCollision(mighter2d::GridObject *door, mighter2d::GridObject *otherGameObject) {

    }

    void CollisionManager::CollisionResolver::resolveSlowDownSensorCollision(mighter2d::GridObject *sensor, mighter2d::GridObject *objectOnSensor) {

    }
}