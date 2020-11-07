/**
 * @brief Class for instantiating score objects
 */

#ifndef SCORE_H
#define SCORE_H

#include <string>

namespace SuperPacMan {
    class Score {
    public:
        /**
         * @brief Constructor
         */
        Score();

        /**
         * @brief Copy constructor
         */
        Score(const Score&) = default;

        /**
         * @brief Assignment operator
         */
        Score& operator=(const Score&) = default;

        /**
         * @brief Check if a score is the same as this score
         * @param rhs Score to check against this score
         * @return True if the scores are the same, otherwise false
         */
        bool operator==(const Score& rhs) const;

        /**
         * @brief Check if a score is not the same as this score
         * @param rhs Score to check against this score
         * @return True if the scores are not the same, otherwise false
         */
        bool operator!=(const Score& rhs) const;

        /**
         * @brief Check if a score has a value less than this scores value
         * @param rhs Score to check against this one
         * @return True if the specified score has a value less than this score
         *         value, otherwise false
         */
        bool operator<(const Score& rhs) const;

        /**
         * @brief Check if a score has a value greater than this scores value
         * @param rhs Score to check against this one
         * @return True if the specified score has a value greater than this score
         *         value, otherwise false
         */
        bool operator>(const Score& rhs) const;

        /**
         * @brief Set the value of the score
         * @param value New value of the score
         *
         * This function will overwrite the previous the score value. To adjust
         * the score value by an offset, @see adjustScore(int). The default value
         * of the score is
         */
        void setValue(int value);

        /**
         * @brief Increase or decrease the score value
         * @param offset The value to increase/decrease the score value by
         *
         * A negative offset decreases the score value while a positive offset
         * increases offset the score
         */
        void adjustValue(int offset);

        /**
         * @brief Set the name of the person the score belongs to
         * @param name Name of the person
         *
         * The score is not associated with any owner (empty string) by default
         */
        void setOwner(const std::string& name);

        /**
         * @brief Get the value of the score
         * @return The value of the score
         */
        int getValue() const;

        /**
         * @brief Get the name of the person the score belongs to
         * @return The name of the person the score belongs to
         */
        const std::string& getOwner() const;

    private:
        //Score points
        int value_;
        //Name of the person the score belongs to
        std::string owner_;
    };
}

#endif
