#include <IME/graphics/Window.h>
#include "../entities/AllEntities.h"
#include "Drawer.h"

namespace SuperPacMan {
    Drawer::Drawer(IME::Graphics::Window &renderTarget) :
            renderTarget_(renderTarget)
    {}

    void Drawer::drawEntities(const std::vector<std::shared_ptr<IME::Entity>> &entities) {
        std::for_each(entities.begin(), entities.end(), [this](auto& entity) {
            if (entity->getClassType() == "Pellet")
                renderTarget_.draw(std::dynamic_pointer_cast<Pellet>(entity)->getSprite());
            else if (entity->getClassType() == "Fruit")
                renderTarget_.draw(std::dynamic_pointer_cast<Fruit>(entity)->getSprite());
            else if (entity->getClassType() == "Key")
                renderTarget_.draw(std::dynamic_pointer_cast<Key>(entity)->getSprite());
            else if (entity->getClassType() == "PacMan")
                renderTarget_.draw(std::dynamic_pointer_cast<PacMan>(entity)->getSprite());
            else if (entity->getClassType() == "Ghost")
                renderTarget_.draw(std::dynamic_pointer_cast<Ghost>(entity)->getSprite());
            else if (entity->getClassType() == "Door")
                renderTarget_.draw(std::dynamic_pointer_cast<Door>(entity)->getSprite());
        });
    }
}
