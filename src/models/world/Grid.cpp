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

#include "src/models/world/Grid.h"
#include "src/animations/GridAnimation.h"
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    Grid::Grid(ime::TileMap &tileMap, ime::Scene& scene, ime::GameObjectContainer& objects) :
        grid_{tileMap},
        scene_{scene},
        actors_{objects},
        spriteSheet_{"spritesheet.png", {224, 244}, {1, 1}, {0, 237, 901, 491}}
    {
        spriteSheet_.assignAlias({0, 1}, "intro_grid");
        spriteSheet_.assignAlias({0, 2}, "level1_to_4_grid");
        spriteSheet_.assignAlias({1, 0}, "level5_to_8_grid");
        spriteSheet_.assignAlias({1, 1}, "level9_to_12_grid");
        spriteSheet_.assignAlias({1, 2}, "level13_to_16_grid");
        spriteSheet_.assignAlias({1, 3}, "level17_to_20_grid");

        // Set render layers for different game object. Note that by default, IME sorts
        // layers by the order in which they are created so there is no need to reorder 
        // if the creation order matches the render order
        grid_.renderLayers().removeByName("default"); // This layer is replaced by the background layer
        grid_.renderLayers().create("background");
        grid_.renderLayers().create("Walls");
        grid_.renderLayers().create("Doors");
        grid_.renderLayers().create("Keys");
        grid_.renderLayers().create("Fruits");
        grid_.renderLayers().create("Pellets");
        grid_.renderLayers().create("Ghosts");
        grid_.renderLayers().create("PacMans");
        grid_.renderLayers().create("Sensors");

        // Create the grid background - Instead of creating the visual grid ourself
        // we will use a pre-made one from an image file and render our game objects on
        // top of it
        grid_.renderLayers().add(background_, 0, "background");

        auto animations = GridAnimation();
        for (const auto& animation : animations.getAll())
            background_.getAnimator().addAnimation(animation);

        background_.getAnimator().on(ime::Animator::Event::AnimationComplete, [this] {
            if (onAnimFinish_)
                onAnimFinish_();
        });

        background_.scale(2.1f, 2.1f);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::loadFromFile(const std::string& filename) {
        grid_.loadFromFile(filename);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::setVisible(bool visible) {
        grid_.getRenderer().setVisible(visible);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::setPosition(ime::Vector2f pos) {
        grid_.setPosition(pos.x, pos.y);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::setBackgroundImagePosition(ime::Vector2f pos) {
        background_.setPosition(pos);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::setBackground(int level) {
        ime::Sprite sprite;
        if (level == 0)
            sprite = spriteSheet_.getSprite("intro_grid");
        else if (level >= 1 && level <= 4)
            sprite = spriteSheet_.getSprite("level1_to_4_grid");
        else if (level >= 5 && level <= 8)
            sprite = spriteSheet_.getSprite("level5_to_8_grid");
        else if (level >= 9 && level <= 12)
            sprite = spriteSheet_.getSprite("level9_to_12_grid");
        else if (level >= 13 && level <= 16)
            sprite = spriteSheet_.getSprite("level13_to_16_grid");
        else
            sprite = spriteSheet_.getSprite("level17_to_20_grid");

        background_.setTexture(sprite.getTexture());
        background_.setTextureRect(sprite.getTextureRect());
    }

    ///////////////////////////////////////////////////////////////
    void Grid::addActor(ime::GameObject::Ptr object, ime::Index index) {
        assert(object && "Object must not be a nullptr");
        std::string renderLayer = object->getClassName() + "s";

        grid_.addChild(object.get(), index);
        auto group = object->getClassName();
        actors_.add(group, std::move(object), 0, renderLayer);
    }

    ///////////////////////////////////////////////////////////////
    ime::GameObject* Grid::getActorById(int id) const {
        return grid_.getChildWithId(id);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::removeActor(int id) {
        grid_.removeChildWithId(id);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::forEachCell(const ime::Callback<const ime::Tile&>& callback) {
        grid_.forEachTile([&callback](const ime::Tile& tile) {
            callback(tile);
        });
    }

    ///////////////////////////////////////////////////////////////
    void Grid::forEachActor(const ime::Callback<ime::GameObject*> &callback) {
        grid_.forEachChild([&callback](ime::GameObject* actor) {
            callback(actor);
        });
    }

    ///////////////////////////////////////////////////////////////
    ime::Time Grid::playFlashAnimation(int level) {
        auto colour = std::string();
        if (level >= 1 && level <= 4)
            colour = "Blue";
        else if (level >= 5 && level <= 8)
            colour = "Orange";
        else if (level >= 9 && level <= 12)
            colour = "Purple";
        else if (level >= 13 && level <= 16)
            colour = "Pink";
        else
            colour = "Green";

        background_.getAnimator().startAnimation("flash" + colour);
        return background_.getAnimator().getActiveAnimation()->getDuration();
    }

    ///////////////////////////////////////////////////////////////
    void Grid::onAnimationFinish(ime::Callback<> callback) {
        onAnimFinish_ = std::move(callback);
    }

    ///////////////////////////////////////////////////////////////
    bool Grid::isAnimationPlaying() {
        return background_.getAnimator().isAnimationPlaying();
    }

    ///////////////////////////////////////////////////////////////
    void Grid::update(ime::Time deltaTime) {
        background_.updateAnimation(deltaTime);
    }

    ///////////////////////////////////////////////////////////////
    ime::Scene& Grid::getScene() {
        return scene_;
    }

} // namespace spm
