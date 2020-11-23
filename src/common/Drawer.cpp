#include <IME/graphics/Window.h>
#include "../entity/AllEntities.h"
#include "Drawer.h"

namespace SuperPacMan {
    Drawer::Drawer(IME::Graphics::Window &renderTarget) :
            renderTarget_(renderTarget)
    {}

    void Drawer::drawEntities(const std::vector<std::shared_ptr<IME::Entity>> &entities) {
        std::for_each(entities.begin(), entities.end(), [this](auto& entity) {
            if (entity->getObjectType() == "Pellet")
                renderTarget_.draw(std::dynamic_pointer_cast<Pellet>(entity)->getSprite());
            else if (entity->getObjectType() == "Fruit")
                renderTarget_.draw(std::dynamic_pointer_cast<Fruit>(entity)->getSprite());
            else if (entity->getObjectType() == "Key")
                renderTarget_.draw(std::dynamic_pointer_cast<Key>(entity)->getSprite());
            else if (entity->getObjectType() == "PacMan")
                renderTarget_.draw(std::dynamic_pointer_cast<PacMan>(entity)->getSprite());
            else if (entity->getObjectType() == "Ghost")
                renderTarget_.draw(std::dynamic_pointer_cast<Ghost>(entity)->getSprite());
            else if (entity->getObjectType() == "Door")
                renderTarget_.draw(std::dynamic_pointer_cast<Door>(entity)->getSprite());
        });
    }
}
