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

#include "SpriteContainer.h"
#include <cassert>

namespace SuperPacMan {
    void SpriteContainer::addSprite(const std::string &name, IME::Graphics::Sprite sprite) {
        sprites_.insert({name, std::move(sprite)});
    }

    IME::Graphics::Sprite SpriteContainer::getSprite(const std::string &name) {
        assert(hasSprite(name) && "Cannot retrieve an unknown sprite from SpriteContainer");
        return sprites_[name];
    }

    bool SpriteContainer::hasSprite(const std::string &name) {
        return sprites_.find(name) != sprites_.end();
    }

    bool SpriteContainer::removeSprite(const std::string &name) {
        if (hasSprite(name)) {
            sprites_.erase(name);
            return true;
        }
        return false;
    }
}
