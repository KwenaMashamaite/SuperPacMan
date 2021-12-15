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

#include "Pellet.h"
#include "Animations/PelletAnimations.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    Pellet::Pellet(ime::Scene& scene, Type type) :
        ime::GameObject(scene)
    {
        setCollisionGroup(type == Type::Power ? "powerPellets" : "superPellets");
        setTag(type == Type::Power ? "power" : "super");

        auto animations = PelletAnimations();
        animations.createAnimationFor((type == Type::Power ? "powerPellet" : "superPellet"));
        getSprite() = animations.getAll().at(0)->getSpriteSheet().getSprite(ime::Index{0, 0});
        for (const auto& animation : animations.getAll())
            getSprite().getAnimator().addAnimation(animation);

        getSprite().getAnimator().startAnimation("blink");
        getSprite().scale(2.0f, 2.0f);
        resetSpriteOrigin(); // Set sprite origin as the centre
    }

    ///////////////////////////////////////////////////////////////
    std::string Pellet::getClassName() const {
        return "Pellet";
    }

} // namespace spm
