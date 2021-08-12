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

#include "src/models/actors/Door.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    Door::Door(ime::Scene& scene) :
        ime::GameObject(scene),
        orientation_(Orientation::Unknown),
        spriteSheet_{"spritesheet.png", {16, 16}, {1, 1}, {368, 17, 86, 18}},
        isBroken_{false}
    {
        spriteSheet_.assignAlias({0, 0}, "unlocked_door");
        spriteSheet_.assignAlias({0, 1}, "locked_door_horizontal");
        spriteSheet_.assignAlias({0, 2}, "broken_door_horizontal");
        spriteSheet_.assignAlias({0, 3}, "locked_door_vertical");
        spriteSheet_.assignAlias({0, 4}, "broken_door_vertical");

        getSprite().setTexture(spriteSheet_.getTexture());
        setDoorTexture("unlocked_door");
        getSprite().scale(2.07f, 2.07f);
        resetSpriteOrigin(); // Set sprite origin as the centre

        setCollisionGroup("doors");
    }

    ///////////////////////////////////////////////////////////////
    void Door::setOrientation(Orientation orientation) {
        orientation_ = orientation;
    }

    ///////////////////////////////////////////////////////////////
    Orientation Door::getOrientation() const {
        return orientation_;
    }

    ///////////////////////////////////////////////////////////////
    void Door::addDoorLocker(DoorLocker doorLocker) {
        doorLocker_ = doorLocker;
    }

    ///////////////////////////////////////////////////////////////
    void Door::lockWith(const Key &key) {
        if (!isLocked()) {
            doorLocker_.lock(key);
            if (doorLocker_.isLocked()) {
                setAsObstacle(true);
                setCollisionId(0); // All collidable actors have an a collision of 0
                if (orientation_ == Orientation::Horizontal)
                    setDoorTexture("locked_door_horizontal");
                else
                    setDoorTexture("locked_door_vertical");
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    void Door::unlockWith(const Key &key) {
        if (isLocked()) {
            doorLocker_.unlock(key);
            if (!doorLocker_.isLocked()) {
                setAsObstacle(false);
                setCollisionId(-1); // Assign door a unique collision id so that it cannot be collided with
                setDoorTexture("unlocked_door");
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    bool Door::isLocked() const {
        return doorLocker_.isLocked();
    }

    ///////////////////////////////////////////////////////////////
    void Door::burstOpen() {
        if (isBroken_)
            return;

        isBroken_ = true;
        setAsObstacle(false);
        setCollisionId(-1); // Assign door unique collision id so that it cannot be collided with
        if (orientation_ == Orientation::Horizontal)
            setDoorTexture("broken_door_horizontal");
        else
            setDoorTexture("broken_door_vertical");
    }

    ///////////////////////////////////////////////////////////////
    bool Door::isBroken() const {
        return isBroken_;
    }

    ///////////////////////////////////////////////////////////////
    void Door::setDoorTexture(const std::string &alias) {
        getSprite().setTextureRect(spriteSheet_.getSprite(alias).getTextureRect());
    }

    ///////////////////////////////////////////////////////////////
    std::string Door::getClassName() const {
        return "Door";
    }

} // namespace spm
