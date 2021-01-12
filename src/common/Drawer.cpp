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

#include <IME/graphics/Window.h>
#include "../entities/AllEntities.h"
#include "Drawer.h"

namespace pacman {
    Drawer::Drawer(ime::Window &renderTarget) :
        renderTarget_(renderTarget)
    {}

    void Drawer::drawEntities(const std::vector<std::shared_ptr<ime::Entity>> &entities) {
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
