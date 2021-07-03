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

#include "src/models/actors/Key.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    Key::Key(ime::Scene& scene, int id) :
        ime::GameObject(scene),
        id_(id)
    {
        getSprite().setTexture("spritesheet.png");
        getSprite().setTextureRect({290, 142, 16, 16});
        resetSpriteOrigin(); // Set sprite origin as the centre
        getTransform().scale(2.0f, 2.0f);

        setCollisionGroup("keys");
    }

    ///////////////////////////////////////////////////////////////
    void Key::setId(int id) {
        id_ = id;
    }

    ///////////////////////////////////////////////////////////////
    int Key::getId() const {
        return id_;
    }

    ///////////////////////////////////////////////////////////////
    std::string Key::getClassName() const {
        return "Key";
    }

} // namespace spm
