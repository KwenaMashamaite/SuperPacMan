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

#include "AudioManager.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    constexpr auto SoundEffect = mighter2d::audio::Type::Sfx;

    ///////////////////////////////////////////////////////////////
    AudioManager::AudioManager(mighter2d::Scene& scene, GameplayObserver& gameplayObserver) :
        audioPlayer_(scene),
        starSpawnSfx_(nullptr),
        bgrndMusic_(nullptr),
        gameplayObserver_(&gameplayObserver)
    {
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::init() {
        gameplayObserver_->onGamePause([this] {
            pause();
        });

        gameplayObserver_->onGameResume([this] {
            resume();
        });

        gameplayObserver_->onExtraLifeAward([this] {
            playOneUpSfx();
        });

        gameplayObserver_->onGameplayDelayEnd([this] {
            playBackgroundMusic(1);
        });

        gameplayObserver_->onKeyEaten([this](Key*) {
            playKeyEatenSfx();
        });

        gameplayObserver_->onPowerPelletEaten([this](Pellet*) {
            playPowerPelletEatenSfx();
        });

        gameplayObserver_->onSuperPelletEaten([this](Pellet*) {
            playSuperPelletEatenSfx();
        });

        gameplayObserver_->onFruitEaten([this](Fruit*) {
            playFruitEatenSfx();
        });

        gameplayObserver_->onGhostEaten([this](Ghost*) {
            playGhostEatenSfx();
        });

        gameplayObserver_->onPacmanDying([this](PacMan*) {
            playPacmanDyingSfx();
        });
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::setVolume(float volume) {
        audioPlayer_.setMasterVolume(volume);
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playKeyEatenSfx() {
        audioPlayer_.play(SoundEffect, "keyEaten.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playFruitEatenSfx() {
        audioPlayer_.play(SoundEffect, "WakkaWakka.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playPowerPelletEatenSfx() {
        audioPlayer_.play(SoundEffect, "powerPelletEaten.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playSuperPelletEatenSfx() {
        audioPlayer_.play(SoundEffect, "superPelletEaten.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playGhostEatenSfx() {
        audioPlayer_.play(SoundEffect, "ghostEaten.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playPacmanDyingSfx() {
        audioPlayer_.play(SoundEffect, "pacmanDying.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playBonusFruitMatchSfx() {
        audioPlayer_.play(SoundEffect, "bonusFruitMatch.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playBonusFruitNotMatchSfx() {
        audioPlayer_.play(SoundEffect, "bonusFruitNotMatch.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playDoorBrokenSfx() {
        audioPlayer_.play(SoundEffect, "doorBroken.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playLevelCompleteSfx() {
        audioPlayer_.play(SoundEffect, "levelComplete.ogg");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playOneUpSfx() {
        audioPlayer_.play(SoundEffect, "extraLife.wav");
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playStarSpawnedSfx() {
        if (!starSpawnSfx_) {
            starSpawnSfx_ = audioPlayer_.play(SoundEffect, "starSpawned.wav");
            starSpawnSfx_->setLoop(true);
        }
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::stopStarSpawnedSfx() {
        if (starSpawnSfx_) {
            starSpawnSfx_->stop();
            starSpawnSfx_ = nullptr;
        }
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::playBackgroundMusic(unsigned int bgMusicNum) {
        stopBackgroundMusic();

        std::string filename;

        if (bgMusicNum == 1)
            filename = "wieu_wieu_slow.ogg";
        else if (bgMusicNum == 2)
            filename = "ghostsTurnedBlue.wav";
        else
            return;

        bgrndMusic_ = audioPlayer_.play(SoundEffect, filename);
        bgrndMusic_->setLoop(true);
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::pauseBackgroundMusic() {
        if (bgrndMusic_) {
            bgrndMusic_->pause();
        }
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::resumeBackgroundMusic() {
        if (bgrndMusic_)
            bgrndMusic_->play();
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::stopBackgroundMusic() {
        if (bgrndMusic_) {
            bgrndMusic_->stop();
            bgrndMusic_ = nullptr;
        }
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::pause() {
        audioPlayer_.pauseAll();
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::resume() {
        audioPlayer_.playAll();
    }

    ///////////////////////////////////////////////////////////////
    void AudioManager::stop() {
        stopStarSpawnedSfx();
        audioPlayer_.stopAll();
    }
}
