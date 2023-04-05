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

#ifndef SUPERPACMAN_AUDIOMANAGER_H
#define SUPERPACMAN_AUDIOMANAGER_H

#include <IME/core/audio/AudioManager.h>

namespace spm {
    /**
     * @brief Plays game audio
     */
    class AudioManager {
    public:
        /**
         * @brief Constructor
         */
        AudioManager();

        /**
         * @brief Set the audio volume
         * @param volume The volume to set
         */
        void setVolume(float volume);

        /**
         * @brief Play key eaten sound effect
         */
        void playKeyEatenSfx();

        /**
         * @brief Play fruit eaten sound effect
         */
        void playFruitEatenSfx();

        /**
         * @brief Play power pellet eaten sound effect
         */
        void playPowerPelletEatenSfx();

        /**
         * @brief Play a super pellet eaten sound effect
         */
        void playSuperPelletEatenSfx();

        /**
         * @brief Play a ghost eaten sound effect
         */
        void playGhostEatenSfx();

        /**
         * @brief Play a pacman dying sound effect
         */
        void playPacmanDyingSfx();

        /**
         * @brief Play a bonus fruit match sound effect
         */
        void playBonusFruitMatchSfx();

        /**
         * @brief Play a bonus fruit not match sound effect
         */
        void playBonusFruitNotMatchSfx();

        /**
         * @brief Play a door broken sound effect
         */
        void playDoorBrokenSfx();

        /**
         * @brief Play a level complete sound effect
         */
        void playLevelCompleteSfx();

        /**
         * @brief Play the one up sound effect
         */
        void playOneUpSfx();

        /**
         * @brief Play star spawned sfx
         *
         * This sound effect is looped
         *
         * @see stopStartSpawnedSfx()
         */
        void playStarSpawnedSfx();

        /**
         * @brief Stop the star spawned sound effect
         */
        void stopStarSpawnedSfx();

        /**
         * @brief Play the game play background music
         * @param bgMusicNum Background music number
         */
        void playBackgroundMusic(unsigned int bgMusicNum);

        /**
         * @brief Pause playing background music
         */
        void pauseBackgroundMusic();

        /**
         * @brief Resume paused background music
         */
        void resumeBackgroundMusic();

        /**
         * @brief Stop the background music
         */
        void stopBackgroundMusic();

        /**
         * @brief Pause all playing audio
         */
        void pause();

        /**
         * @brief Resume all paused audio
         */
        void resume();

        /**
         * @brief Stop all playing audio
         */
        void stop();

    private:
        ime::audio::AudioManager audioPlayer_;
        ime::audio::Audio* starSpawnSfx_;
        ime::audio::Audio* bgrndMusic_;
    };
}

#endif //SUPERPACMAN_AUDIOMANAGER_H
