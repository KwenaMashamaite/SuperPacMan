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

namespace spm {
    ///////////////////////////////////////////////////////////////
    PelletAnimations::PelletAnimations() :
        spritesheet_{"spritesheet.png", {16, 16}, {1, 1}, {249, 17, 120, 18}}
    {}

    ///////////////////////////////////////////////////////////////
    void PelletAnimations::createAnimationFor(const std::string& tag) {
        auto blinkAnimation = ime::Animation::create("blink", spritesheet_, ime::milliseconds(300));
        blinkAnimation->setLoop(true);

        if (tag == "powerPellet")
            blinkAnimation->addFrames({0, 0}, 2);
        else if (tag == "superPellet") {
            blinkAnimation->setDuration(ime::seconds(1));
            blinkAnimation->addFrames({0, 1}, 6);
        } else
            return;

        animations_.push_back(std::move(blinkAnimation));
    }

    ///////////////////////////////////////////////////////////////
    const std::vector<ime::Animation::Ptr> & PelletAnimations::getAll() const {
        return animations_;
    }

} // namespace spm