/**
 * @brief Class for loading and persisting the games scores
 */

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Score.h"
#include <vector>
#include <string>

namespace SuperPacMan {
    class Scoreboard {
    public:
        using ScoreContainer = std::vector<Score>;
        using constIterator = ScoreContainer::const_iterator;

        /**
         * @brief Constructor
         * @param filename The file name (including path) of the file that
         *        contains the highscores
         * @throw FileNotFound If the file cannot be found on the disk
         */
        explicit Scoreboard(const std::string &filename);

        /**
         * @brief Load high scores from the disk
         * @throws FileNotFound if the file containing high scores cannot be
         *         found on the disk
         *
         * The high scores will be loaded from the file provided during instantiation
         */
        void load();

        /**
         * @brief Add points to the current score
         * @param points Points to add
         *
         * This function does not overwrite the existing current score,
         * it adds to it. Providing a negative score subtracts from the
         * current score
         */
        void addPoints(int points);

        /**
         * @brief Write scores to highscores file on disk
         * @param name Name of the person the current score belongs to
         *
         * This function will update the file provided on instantiation if
         * necessary. The file is updated only if the current score is
         * greater than the lowest highscore from the last file read
         */
        void updateHighScoreFile(const std::string& name);

        /**
         * @brief Reset the current score to zero
         */
        void resetCurrentScore();

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
         * @brief  Get a constant iterator that points to the first element
         *         in a high scores container
         * @return A constant iterator that points to the first element in
         *         a high scores container
         */
        constIterator cBegin() const;

        /**
         * @brief  Get a constant iterator that points one past the last
         *         element in a high scores container
         * @return A constant iterator that points one past the last
         *         element in a high scores container
         */
        constIterator cEnd() const;

    private:
        //stores high scores read from disk file
        ScoreContainer highScores_;
        //Stores current score
        Score currentScore_;
        //highscores file path on the disk
        std::string highScoresFile_;
    };
}

#endif
