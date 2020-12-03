#include "ScatterState.h"
#include <cassert>

const auto topLeftPath = std::vector<IME::Index>{{7, 3}, {5, 5}, {3, 3}};
const auto topRightPath = std::vector<IME::Index>{{5, 21}, {7, 19}, {3, 19}};
const auto bottomLeftPath = std::vector<IME::Index>{{25, 3}, {23, 5}, {21, 3}};
const auto bottomRightPath = std::vector<IME::Index>{{21, 21}, {21, 19}, {25, 19}};

std::queue<IME::Index> vectorToQueue(const std::vector<IME::Index>& vector) {
    std::queue<IME::Index> queue;
    for (const auto& index : vector)
        queue.push(index);
    return queue;
}

namespace SuperPacMan {
    ScatterState::ScatterState(ScatterPosition scatterPos, std::shared_ptr<IME::Entity> ghost, IME::TileMap &grid) :
        targetPos_(scatterPos),
        ghostMover_(grid, ghost)
    {
        assert(std::dynamic_pointer_cast<Ghost>(ghost) && "Cannot create ghost state for non ghost object");
        ghost_ = std::move(std::dynamic_pointer_cast<Ghost>(ghost));
    }

    void ScatterState::onEntry() {
        switch (targetPos_) {
            case ScatterPosition::TopLeftCorner:
                ghostMover_.setDestination(IME::Index{3, 3});
                break;
            case ScatterPosition::TopRightCorner:
                ghostMover_.setDestination(IME::Index{3, 19});
                break;
            case ScatterPosition::BottomLeftCorner:
                ghostMover_.setDestination(IME::Index{21, 3});
                break;
            case ScatterPosition::BottomRightCorner:
                ghostMover_.setDestination(IME::Index{25, 19});
                break;
        }

        ghostMover_.onDestinationReached([this](IME::Graphics::Tile tile) {
            if (ghostPath_.empty()) {
                switch (targetPos_) {
                    case ScatterPosition::TopLeftCorner:
                        ghostPath_ = std::move(vectorToQueue(topLeftPath));
                        break;
                    case ScatterPosition::TopRightCorner:
                        ghostPath_ = std::move(vectorToQueue(topRightPath));
                        break;
                    case ScatterPosition::BottomLeftCorner:
                        ghostPath_ = std::move(vectorToQueue(bottomLeftPath));
                        break;
                    case ScatterPosition::BottomRightCorner:
                        ghostPath_ = std::move(vectorToQueue(bottomRightPath));
                        break;
                }
            }
            ghostMover_.setDestination(ghostPath_.front());
            ghostPath_.pop();
        });

        ghostMover_.startMovement();
    }

    void ScatterState::update(float deltaTime) {
        ghostMover_.update(deltaTime);
    }

    void ScatterState::onExit() {
        ghostMover_.setTarget(nullptr);
    }
}