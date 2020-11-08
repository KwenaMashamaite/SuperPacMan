#include "SpriteContainer.h"
#include <cassert>

namespace SuperPacMan {
    IME::Graphics::Sprite SpriteContainer::getSprite(const std::string &name) {
        assert(hasSprite(name) && "Cannot retrieve an unknown sprite from SpriteContainer");
        return sprites_[name];
    }

    bool SpriteContainer::hasSprite(const std::string &name) {
        return sprites_.find(name) != sprites_.end();
    }

    void SpriteContainer::addSprite(const std::string &name, IME::Graphics::Sprite sprite) {
        sprites_.insert({name, std::move(sprite)});
    }

    bool SpriteContainer::removeSprite(const std::string &name) {
        if (hasSprite(name)) {
            sprites_.erase(name);
            return true;
        }
        return false;
    }
}
