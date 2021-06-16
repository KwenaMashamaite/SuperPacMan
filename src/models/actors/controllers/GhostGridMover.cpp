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

#include "src/models/actors/controllers/GhostGridMover.h"
#include <IME/utility/Utils.h>

namespace spm {
    GhostGridMover::GhostGridMover(ime::TileMap& tileMap, ime::GameObject* ghost) :
        ime::TargetGridMover(tileMap, ghost),
        destinationReachedId_{-1}
    {}

    void GhostGridMover::generateRandomDestination() {
        static auto generateRandomRow = ime::utility::createRandomNumGenerator(0, getGrid().getSizeInTiles().y);
        static auto generateRandomColm = ime::utility::createRandomNumGenerator(0, getGrid().getSizeInTiles().x);

        ime::Index newDestination;
        do {
            newDestination = ime::Index{generateRandomRow(), generateRandomColm()};
        } while(!isDestinationReachable(newDestination));

        setDestination(newDestination);
    }

    void GhostGridMover::setRandomMoveEnable(bool enable) {
        if (destinationReachedId_ != -1 && enable)
            return;

        unsubscribe(destinationReachedId_);
        destinationReachedId_ = -1;

        if (enable) {
            generateRandomDestination(); // Generate initial random position

            // Generate a new random position when target reaches its current destination
            destinationReachedId_ = onDestinationReached([this](ime::Index) {
                generateRandomDestination();
            });
        }
    }
}
