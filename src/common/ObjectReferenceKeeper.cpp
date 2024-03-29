////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
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

#include "ObjectReferenceKeeper.h"
#include <cassert>
#include <algorithm>

namespace spm {
    ///////////////////////////////////////////////////////////////
    void ObjectReferenceKeeper::registerGameObject(ime::GridObject *gameObject) {
        assert(gameObject);
        gameObjects_.insert({gameObject->getTag(), gameObject});
    }

    ///////////////////////////////////////////////////////////////
    void ObjectReferenceKeeper::deregisterGameObject(ime::GridObject *gameObject) {
        assert(gameObject);
        auto found = std::find_if(gameObjects_.begin(), gameObjects_.end(), [gameObject](auto& pair) {
            return pair.second->getObjectId() == gameObject->getObjectId();
        });

        if (found != gameObjects_.end())
            gameObjects_.erase(found);
    }

    ///////////////////////////////////////////////////////////////
    ime::GridObject* ObjectReferenceKeeper::getActor(const std::string &tag) {
        auto found = gameObjects_.find(tag);
        return found != gameObjects_.end() ? found->second : nullptr;
    }

    ///////////////////////////////////////////////////////////////
    void ObjectReferenceKeeper::clear() {
        gameObjects_.clear();
    }

} // namespace pm
