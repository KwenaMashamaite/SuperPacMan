////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#include "Door.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    static int counter = 0;

    ///////////////////////////////////////////////////////////////
    bool isValidKey(const Key& key, int doorId) {
        switch (key.getId()) {
            case 1: return doorId == 1 || doorId == 6;
            case 2: return doorId == 3 || doorId == 7;
            case 3: return doorId == 2 || doorId == 4;
            case 4: return doorId == 5;
            case 5: return doorId == 10 || doorId == 13 || doorId == 14;
            case 6: return doorId == 11 || doorId == 17 || doorId == 18;
            case 7: return doorId == 20 || doorId == 26;
            case 8: return doorId == 15 || doorId == 21 || doorId == 24;
            case 9: return doorId == 16 || doorId == 22 || doorId == 25;
            case 10: return doorId == 23 || doorId == 27;
            case 11: return doorId == 12 || doorId == 19 || doorId == 30;
            case 12: return doorId == 29 || doorId == 34 || doorId == 36;
            case 13: return doorId == 31 || doorId == 37;
            case 14: return doorId == 28 || doorId == 33;
            case 15: return doorId == 32 || doorId == 35;
            default:
                return false;
        }
    }

    ///////////////////////////////////////////////////////////////
    Door::Door(mighter2d::Scene& scene) :
        mighter2d::GridObject(scene),
        id_{++counter},
        isLocked_{false},
        orientation_(Orientation::Vertical),
        spriteSheet_{"spritesheet.png", {16, 16}, {1, 1}, {368, 17, 86, 18}}
    {
        setCollisionGroup("doors");
        spriteSheet_.assignAlias({0, 0}, "unlocked_door");
        spriteSheet_.assignAlias({0, 1}, "locked_door_horizontal");
        spriteSheet_.assignAlias({0, 2}, "broken_door_horizontal");
        spriteSheet_.assignAlias({0, 3}, "locked_door_vertical");
        spriteSheet_.assignAlias({0, 4}, "broken_door_vertical");

        getSprite().setTexture(spriteSheet_.getTexture());
        setDoorTexture("unlocked_door");
        getSprite().scale(2.07f, 2.07f);

        // Collision detection
        setStatic(true);
    }

    ///////////////////////////////////////////////////////////////
    void Door::resetCounter() {
        counter = 0;
    }

    ///////////////////////////////////////////////////////////////
    void Door::setOrientation(Orientation orientation) {
        orientation_ = orientation;
    }

    ///////////////////////////////////////////////////////////////
    void Door::lock() {
        if (!isLocked_) {
            isLocked_ = true;
            setObstacle(true);
            setCollisionId(0); // Enable collisions with other game objects

            if (orientation_ == Orientation::Horizontal)
                setDoorTexture("locked_door_horizontal");
            else
                setDoorTexture("locked_door_vertical");
        }
    }

    ///////////////////////////////////////////////////////////////
    void Door::unlock(const Key &key) {
        if (isLocked_ && isValidKey(key, id_)) {
            isLocked_ = false;
            setObstacle(false);
            setCollisionId(-1); // Disable collisions with other game objects
            setDoorTexture("unlocked_door");
        }
    }

    ///////////////////////////////////////////////////////////////
    bool Door::isLocked() const {
        return isLocked_;
    }

    ///////////////////////////////////////////////////////////////
    void Door::burst() {
        setObstacle(false);
        setCollisionId(-1); // Disable collisions with other game objects

        if (orientation_ == Orientation::Horizontal)
            setDoorTexture("broken_door_horizontal");
        else
            setDoorTexture("broken_door_vertical");
    }

    ///////////////////////////////////////////////////////////////
    void Door::setDoorTexture(const std::string &alias) {
        getSprite().setTextureRect(spriteSheet_.getSprite(getScene(), alias).getTextureRect());
    }

    ///////////////////////////////////////////////////////////////
    std::string Door::getClassName() const {
        return "Door";
    }

} // namespace spm
