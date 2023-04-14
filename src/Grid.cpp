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
#include <Mighter2d/core/scene/Scene.h>
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    Grid::Grid(mighter2d::Grid &grid) :
        grid_{grid},
        spriteSheet_{"spritesheet.png", {224, 244}, {1, 1}, {0, 237, 901, 491}},
        background_(getScene())
    {
        // Set up render layers
        mighter2d::RenderLayerContainer& renderLayers = grid.getScene().getRenderLayers();
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

        // background
        background_.setTexture(spriteSheet_.getTexture());
        background_.scale(2.1f, 2.1f);

        // Set up animations
        GridAnimation animations;

        for (const auto& animation : animations.getAll())
            background_.getAnimator().addAnimation(animation);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::create(int level) {
        grid_.loadFromFile("res/TextFiles/Mazes/GameplayMaze.txt");
        grid_.setPosition(-42, 0);
        setBackground(level);
        background_.setPosition(mighter2d::Vector2f{7.0f, 48.0f});
#ifndef NDEBUG
        grid_.getRenderer().setVisible(true);
#else
        grid_.getRenderer().setVisible(false);
#endif
    }

    ///////////////////////////////////////////////////////////////
    void Grid::setBackground(int level) {
        if (level == 0)
            background_.setTextureRect(*spriteSheet_.getFrame("intro_grid"));
        else if (level >= 1 && level <= 4)
            background_.setTextureRect(*spriteSheet_.getFrame("level_1_to_4_grid"));
        else if (level >= 5 && level <= 8)
            background_.setTextureRect(*spriteSheet_.getFrame("level_5_to_8_grid"));
        else if (level >= 9 && level <= 12)
            background_.setTextureRect(*spriteSheet_.getFrame("level_9_to_12_grid"));
        else if (level >= 13 && level <= 16)
            background_.setTextureRect(*spriteSheet_.getFrame("level_13_to_16_grid"));
        else
            background_.setTextureRect(*spriteSheet_.getFrame("level_17_to_20_grid"));
    }

    ///////////////////////////////////////////////////////////////
    void Grid::addGameObject(mighter2d::GridObject *object, mighter2d::Index index) {
        std::string renderLayer = object->getClassName() + "s";
        const static int renderOrder = 0;

        grid_.getScene().getRenderLayers().add(object->getSprite(), renderOrder, renderLayer);
        grid_.addChild(object, index);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::removeGameObject(mighter2d::GridObject *gameObject) {
        grid_.removeChild(gameObject);
    }

    ///////////////////////////////////////////////////////////////
    void Grid::forEachCell(const mighter2d::Callback<const mighter2d::Tile&>& callback) {
        grid_.forEachTile([&callback](const mighter2d::Tile& tile) {
            callback(tile);
        });
    }

    ///////////////////////////////////////////////////////////////
    void Grid::forEachGameObject(const mighter2d::Callback<mighter2d::GridObject*> &callback) {
        grid_.forEachChild([&callback](mighter2d::GridObject* actor) {
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
    void Grid::onFlashStop(mighter2d::Callback<> callback) {
        background_.getAnimator().onAnimComplete([callback = std::move(callback)] (mighter2d::Animation*) {
            callback();
        });
    }

    ///////////////////////////////////////////////////////////////
    bool Grid::isFlashing() {
        return background_.getAnimator().isAnimationPlaying();
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::Scene& Grid::getScene() {
        return grid_.getScene();
    }

    ///////////////////////////////////////////////////////////////
    Grid::operator mighter2d::Grid& () {
        return grid_;
    }

} // namespace spm
