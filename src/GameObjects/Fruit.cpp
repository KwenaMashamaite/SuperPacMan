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

#include "Fruit.h"
#include "Animations/FruitAnimation.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    Fruit::Fruit(mighter2d::Scene& scene) :
        mighter2d::GridObject(scene)
    {
        auto animation = FruitAnimation();
        getSprite().getAnimator().addAnimation(animation.getAnimation());
        getSprite().setTexture(animation.getAnimation()->getSpriteSheet().getTexture());
        getSprite().scale(2.0f, 2.0f);

        // When the tag of the object is set to any of the following, its sprite
        // will be set to display the corresponding texture, otherwise an empty texture
        auto tags = std::vector {
            "apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach",
            "melon", "coffee", "mushroom", "bell", "clover", "galaxian", "gift"
        };

        auto spritesheet = animation.getAnimation()->getSpriteSheet();
        for (auto colm = 0; colm < tags.size(); ++colm)
            spritesheet.assignAlias({0, colm}, tags.at(colm));

        // Change the sprite texture every time its tag changes to an appropriate image
        onPropertyChange("tag", [this, spritesheet = std::move(spritesheet)](const mighter2d::Property& tag) {
            getSprite().setTextureRect(spritesheet.getSprite(getScene(), tag.getValue<std::string>()).getTextureRect());
        });

        setTag("apple"); // Set apple texture by default
    }

    ///////////////////////////////////////////////////////////////
    std::string Fruit::getClassName() const {
        return "Fruit";
    }

} // namespace spm
