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
        ime::GameObject::Ptr createDoor(const ime::Tile& tile, ime::Scene& scene) {
            auto door = std::make_shared<Door>(scene);
            if (tile.getIndex().row % 2 == 0)
                door->setOrientation(Orientation::Horizontal);
            else
                door->setOrientation(Orientation::Vertical);

            return door;
        }

        void attachRigidBody(const ime::GameObject::Ptr& actor, ime::World& physics, ime::Body::Type bodyType) {
            auto bounds = actor->getSprite().getGlobalBounds();
            auto physicsBody = physics.createBody(bodyType);
            auto collider = ime::BoxCollider::create({bounds.width, bounds.height});
            physicsBody->attachCollider(std::move(collider));
            actor->attachRigidBody(std::move(physicsBody));
        }
    }

    void ObjectCreator::createObjects(ime::World& world, Grid &grid) {
        grid.forEachCell([&world, &grid](const ime::Tile& tile) {
            ime::GameObject::Ptr actor;
            ime::Body::Type rigidBodyType = ime::Body::Type::Static;

            if (tile.getId() == 'X')
                actor = std::make_shared<PacMan>(grid.getScene());
            else if (tile.getId() == 'K')
                actor = std::make_shared<spm::Key>(grid.getScene());
            else if (tile.getId() == 'F')
                actor = std::make_shared<Fruit>(grid.getScene());
            else if (tile.getId() == 'E')
                actor = std::make_shared<Pellet>(grid.getScene(), Pellet::Type::Power);
            else if (tile.getId() == 'S')
                actor = std::make_shared<Pellet>(grid.getScene(), Pellet::Type::Super);
            else if (tile.getId() == 'D')
                actor = createDoor(tile, grid.getScene());
            else {
                if (tile.getId() == 'B')
                    actor = std::make_shared<Ghost>(grid.getScene(), Ghost::Colour::Red);
                else if (tile.getId() == 'P')
                    actor = std::make_shared<Ghost>(grid.getScene(), Ghost::Colour::Pink);
                else if (tile.getId() == 'I')
                    actor = std::make_shared<Ghost>(grid.getScene(), Ghost::Colour::Cyan);
                else if (tile.getId() == 'C')
                    actor = std::make_shared<Ghost>(grid.getScene(), Ghost::Colour::Orange);
                else
                    return;
            }

            attachRigidBody(actor, world, rigidBodyType);
            grid.addObject(std::move(actor), tile.getIndex());
        });
    }
}
