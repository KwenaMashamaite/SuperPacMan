#include "Scoreboard.h"
#include <IME/utility/DiskFileReader.h>
#include <algorithm>

namespace SuperPacMan {
    Scoreboard::Scoreboard(const std::string &filename) : highScoresFile_(filename)
    {}

    void Scoreboard::load() {
        auto highScores = std::stringstream();
        IME::Utility::DiskFileReader().readFileInto(highScoresFile_, highScores);
        auto line = std::string();
        while (std::getline(highScores, line)) {
            auto spacePos = line.find_first_of(' ');
            auto score = Score();
            score.setOwner(line.substr(0, spacePos));
            score.setValue(std::stoi(line.substr(spacePos + 1)));
            highScores_.push_back(score);
        }
    }

    void Scoreboard::addPoints(int points) {
        currentScore_.adjustValue(points);
    }

    void Scoreboard::updateHighScoreFile(const std::string& name) {
        if (currentScore_.getValue() > highScores_.back().getValue()) {//Highscores stored in descending order
            highScores_.pop_back();
            currentScore_.setOwner(name);
            highScores_.push_back(currentScore_);
            std::sort(std::begin(highScores_), std::end(highScores_),std::greater<>());
            auto newHighscoreList = std::stringstream();
            newHighscoreList << highScores_.front().getOwner() + " " + std::to_string(highScores_.front().getValue());
            std::for_each(++highScores_.begin(), highScores_.end(),[&](auto& score) {
                newHighscoreList << "\n" + score.getOwner() + " " + std::to_string(score.getValue());
            });
            IME::Utility::DiskFileReader().writeToFile(newHighscoreList, highScoresFile_);
        }
    }

    void Scoreboard::resetCurrentScore() {
        currentScore_.setValue(0);
    }

    int Scoreboard::getCurrentScore() const {
        return currentScore_.getValue();
    }

    const Score& Scoreboard::getTopScore() const {
        return highScores_.front();
    }

    Scoreboard::constIterator Scoreboard::cBegin() const {
        return highScores_.cbegin();
    }

    Scoreboard::constIterator Scoreboard::cEnd() const {
        return highScores_.cend();
    }
}
