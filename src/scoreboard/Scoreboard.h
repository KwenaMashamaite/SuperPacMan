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

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Score.h"
#include <vector>
#include <string>
#include <functional>

namespace SuperPacMan {
    /**
     * @brief Loads and persists game top scores
     */
    class Scoreboard {
    public:
        /**
         * @brief Constructor
         * @param filename The name of the file that contains the high scores
         *
         * @a filename must be preceded by the path
         */
        explicit Scoreboard(const std::string &filename);

        /**
         * @brief Load high scores from the disk
         * @throws FileNotFound If the file cannot be found on the disk
         *
         * The high scores will be loaded from the file provided during
         * instantiation
         */
        void load();

        /**
         * @brief Add points to the current score
         * @param points Points to add
         *
         * This function adds to the current score. Negative points
         * subtracts from the current score
         */
        void addPoints(int points);

        /**
         * @brief Get the current score
         * @return Current score
         */
        int getCurrentScore() const;

        /**
         * @brief Get the highest score
         * @return Highest score
         */
        const Score& getTopScore() const;

        /**
         * @brief Reset the current score to zero
         */
        void resetCurrentScore();

        /**
         * @brief Get the number of top scores in the scoreboard
         * @return The number of top scores in the scoreboard
         */
        std::size_t getSize() const;

        /**
         * @brief Write scores to a file on the disk
         * @param name Name of the player the current score belongs to
         *
         * This function will write the current top scores to the file
         * provided during instantiation. The file is updated only if
         * the current score is greater than the lowest high score from
         * the last file read
         */
        void updateHighScoreFile(const std::string& name);

        /**
         * @brief Execute a function for each score in the scoreboard
         * @param callback Function to be executed
         */
        void forEachScore(std::function<void(const Score&)> callback);

    private:
        std::vector<Score> highScores_; //!< High scores read from dis
        Score currentScore_;            //!< Stores current score
        std::string highScoresFile_;    //!< High scores file to be read/written
    };
}

#endif
