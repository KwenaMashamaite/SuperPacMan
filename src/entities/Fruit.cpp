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

namespace SuperPacMan {
    Fruit::Fruit(const IME::Vector2u &boundingRect, const std::string &name) :
        Entity(boundingRect, IME::Entity::Type::Collectable),
        name_(name),
        isEaten_(false)
    {
        setCollidable(true);
        sprite_ = SpriteContainer::getSprite(name);
        sprite_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
        sprite_.scale(2.0f, 2.0f);

        onEvent("positionChange", IME::Callback<float, float>([this](float x, float y) {
            sprite_.setPosition(x + getSize().x / 2.0f, y + getSize().y / 2.0f);
        }));
    }

    const std::string &Fruit::getName() const {
        return name_;
    }

    std::string SuperPacMan::Fruit::getClassType() {
        return "Fruit";
    }

    IME::Graphics::Sprite &Fruit::getSprite() {
        return sprite_;
    }

    void Fruit::eat() {
        if (!isEaten_ && isVulnerable()){
            isEaten_ = true;
            setActive(false);
            setCollidable(false);
            publishEvent("eaten");
        }
    }

    bool Fruit::isEaten() const {
        return isEaten_;
    }
}
