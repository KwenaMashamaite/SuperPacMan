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

#include "PelletAnimations.h"

namespace pacman {
    void PelletAnimations::createAnimationFor(PelletType pelletType) {
        auto blinkAnimation = std::make_shared<ime::Animation>("blink", "spritesheet.png", 0.3f);
        blinkAnimation->setLoop(true);
        if (pelletType == PelletType::PowerPellet)
            blinkAnimation->addFrames({151, 18}, {16, 16}, 2);
        else if (pelletType == PelletType::SuperPellet) {
            blinkAnimation->setDuration(1.0f);
            blinkAnimation->addFrames({168, 35}, {16, 16}, 6);
        }
        animations_.push_back(std::move(blinkAnimation));
    }

    const std::vector<std::shared_ptr<ime::Animation>> & PelletAnimations::getAll() const {
        return animations_;
    }
}