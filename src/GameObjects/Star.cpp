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

#include "Star.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    Star::Star(mighter2d::Scene &scene) :
        GridObject(scene)
    {
        setTag("star");

        mighter2d::SpriteSheet spriteSheet{"spritesheet.png", mighter2d::Vector2u{16, 16}, {1, 1}, {289, 209, 35, 18}};
        getSprite().setTexture(spriteSheet.getTexture());

        mighter2d::Animation::Ptr blinkAnim = mighter2d::Animation::create("blink", spriteSheet, mighter2d::seconds(0.27));
        blinkAnim->addFrames(mighter2d::Index{0, 0}, 2);
        blinkAnim->setLoop(true);
        getSprite().setTexture(blinkAnim->getSpriteSheet().getTexture());
        getSprite().getAnimator().addAnimation(std::move(blinkAnim));

        getSprite().scale(2.0f, 2.0f);
        getSprite().getAnimator().startAnimation("blink");

        // Collision detection
        setStatic(true);
    }

    ///////////////////////////////////////////////////////////////
    std::string Star::getClassName() const {
        return "Star";
    }
}
