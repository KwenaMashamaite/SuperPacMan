

#ifndef ALIASES_H
#define ALIASES_H

#include <vector>
#include <memory>

namespace IME {
    class Entity;

    namespace Graphics {
        class Sprite;
        class AnimatableSprite;
    }
}

namespace SuperPacMan {
    using StaticSpriteContainer =  std::vector<std::shared_ptr<IME::Graphics::Sprite>>;
    using AnimatableSpriteContainer = std::vector<std::shared_ptr<IME::Graphics::AnimatableSprite>>;
    using EntityContainer = std::vector<std::shared_ptr<IME::Entity>>;
}

#endif