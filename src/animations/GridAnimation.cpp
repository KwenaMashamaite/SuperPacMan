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

#include "GridAnimation.h"

namespace pacman {
    GridAnimation::GridAnimation() {
        createFlashAnimations("blue", {450, 0, 224, 244});
        createFlashAnimations("orange", {0, 249, 224, 244});
        createFlashAnimations("purple", {225, 249, 224, 244});
        createFlashAnimations("pink", {450, 249, 224, 244});
        createFlashAnimations("green", {675, 249, 224, 244});
    }

    const std::vector<std::shared_ptr<ime::Animation>> & GridAnimation::getAnimations() {
        return animations_;
    }

    void GridAnimation::createFlashAnimations(const std::string& gridColour, ime::IntRect gridFrame) {
        auto animation = std::make_shared<ime::Animation>("flash-" + gridColour, "grids.png", 3.0f);
        for (auto i = 0; i < 15; ++i) //Add the same frame multiple time because animation is not looped and should play really fast
            animation->addFrames({gridFrame, {675, 0, 224, 244}});
        animations_.push_back(std::move(animation));
    }
}
