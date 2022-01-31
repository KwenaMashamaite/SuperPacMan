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

#include "Grid.h"
#include "Animations/GridAnimation.h"
#include "Utils/ObjectCreator.h"
#include <IME/core/scene/Scene.h>
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    Grid::Grid(ime::Grid2D &grid) :
        grid_{grid},
        spriteSheet_{"spritesheet.png", {224, 244}, {1, 1}, {0, 237, 901, 491}}
    {
        // Set up render layers
        ime::RenderLayerContainer& renderLayers = grid.getScene().getRenderLayers();
        renderLayers.removeByName("default"); // This layer is replaced by the background layer

        // Instead of creating the visual grid ourselves, we use a pre-made
        // one from an image file and render game objects on top of it
        renderLayers.create("background");
        renderLayers.add(background_, 0, "background");

        renderLayers.create("Walls");
        renderLayers.create("Sensors");
        renderLayers.create("Doors");
        renderLayers.create("Keys");
        renderLayers.create("Fruits");
        renderLayers.create("Pellets");
        renderLayers.create("Stars");
        renderLayers.create("GridObjects");
        renderLayers.create("Ghosts");
        renderLayers.create("PacMans");
        
        // Set textures for different levels
        spriteSheet_.assignAlias({0, 1}, "intro_grid");
        spriteSheet_.assignAlias({0, 2}, "level_1_to_4_grid");
        spriteSheet_.assignAlias({1, 0}, "level_5_to_8_grid");
        spriteSheet_.assignAlias({1, 1}, "level_9_to_12_grid");
        spriteSheet_.assignAlias({1, 2}, "level_13_to_16_grid");
        spriteSheet_.assignAlias({1, 3}, "level_17_to_20_grid");

        // Set up animations
        GridAnimation animations;
        
        for (const auto& animation : animations.getAll())
            background_.getAnimator().addAnimation(animation);

        background_.scale(2.1f, 2.1f);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::init() {
        ObjectCreator::createObjects(*this);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::create(int level) {
        grid_.loadFromFile("res/TextFiles/Mazes/GameplayMaze.txt");
        grid_.setPosition(-42, 0);
        setBackground(level);
        background_.setPosition(ime::Vector2f{7.0f, 48.0f});
#ifndef NDEBUG
        grid_.getRenderer().setVisible(true);
#else
        grid_.getRenderer().setVisible(false);
#endif
    }

    ///////////////////////////////////////////////////////////////
    void Grid::setBackground(int level) {
        ime::Sprite sprite;
        if (level == 0)
            sprite = spriteSheet_.getSprite("intro_grid");
        else if (level >= 1 && level <= 4)
            sprite = spriteSheet_.getSprite("level_1_to_4_grid");
        else if (level >= 5 && level <= 8)
            sprite = spriteSheet_.getSprite("level_5_to_8_grid");
        else if (level >= 9 && level <= 12)
            sprite = spriteSheet_.getSprite("level_9_to_12_grid");
        else if (level >= 13 && level <= 16)
            sprite = spriteSheet_.getSprite("level_13_to_16_grid");
        else
            sprite = spriteSheet_.getSprite("level_17_to_20_grid");

        background_.setTexture(sprite.getTexture());
        background_.setTextureRect(sprite.getTextureRect());
    }

    ///////////////////////////////////////////////////////////////
    void Grid::addGameObject(ime::GridObject::Ptr object, ime::Index index) {
        assert(object && "Object must not be a nullptr");

        grid_.addChild(object.get(), index);
        std::string renderLayer = object->getClassName() + "s";
        std::string group = object->getClassName();
        grid_.getScene().getGameObjects().add(group, std::move(object), 0, renderLayer);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::addGameObject(ime::GridObject *object, ime::Index index) {
        grid_.addChild(object, index);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::removeGameObject(ime::GridObject *gameObject) {
        grid_.removeChild(gameObject);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::forEachCell(const ime::Callback<const ime::Tile&>& callback) {
        grid_.forEachTile([&callback](const ime::Tile& tile) {
            callback(tile);
        });
    }

    ///////////////////////////////////////////////////////////////
    void Grid::forEachGameObject(const ime::Callback<ime::GridObject*> &callback) {
        grid_.forEachChild([&callback](ime::GridObject* actor) {
            callback(actor);
        });
    }

    ///////////////////////////////////////////////////////////////
    void Grid::flash(int level) {
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
    }

    ///////////////////////////////////////////////////////////////
    void Grid::onFlashStop(ime::Callback<> callback) {
        background_.getAnimator().onAnimComplete([callback = std::move(callback)] (ime::Animation*) {
            callback();
        });
    }

    ///////////////////////////////////////////////////////////////
    bool Grid::isFlashing() {
        return background_.getAnimator().isAnimationPlaying();
    }

    ///////////////////////////////////////////////////////////////
    void Grid::update(ime::Time deltaTime) {
        background_.updateAnimation(deltaTime);
    }

    ///////////////////////////////////////////////////////////////
    ime::Scene& Grid::getScene() {
        return grid_.getScene();
    }

    ///////////////////////////////////////////////////////////////
    Grid::operator ime::Grid2D& () {
        return grid_;
    }

} // namespace spm
