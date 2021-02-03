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

#include "Fruit.h"
#include "../common/SpriteContainer.h"
#include "../animations/FruitAnimation.h"

namespace pacman {
    Fruit::Fruit(const ime::Vector2u &boundingRect, const std::string &name) :
        Entity(boundingRect, ime::Entity::Type::Collectable),
        isEaten_(false)
    {
        setName(name);
        setCollidable(true);
        auto animation = FruitAnimation();
        getSprite().getAnimator().addAnimation(animation.getAnimation());
        getSprite() = SpriteContainer::getSprite(name);

        onEvent("nameChange", ime::Callback<std::string>([this](std::string name) {
            getSprite() = SpriteContainer::getSprite(name);
        }));
    }

    std::string pacman::Fruit::getClassType() {
        return "Fruit";
    }

    void Fruit::eat() {
        if (!isEaten_ && isVulnerable()){
            isEaten_ = true;
            setActive(false);
            setCollidable(false);
            dispatchEvent("eaten");
        }
    }

    bool Fruit::isEaten() const {
        return isEaten_;
    }
}
