#include <MusicManager.h>

MusicManager* MusicManager::musicManagerInstance = nullptr;

//const char mainMelodyString[] = "Tetris:d=4,o=5,b=120:e6,8b,8c6,d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,e6,8b,8c6,d6,8c6,32b.,32c6.,32b.,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,32f6.,32g6.,32f6.,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,2e5,2c5,2d5,2p,2c5,2a,1g#,32p,e6,8b,8c6,d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,e6,8b,8c6,d6,8c6,32b.,32c6.,32b.,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,32f6.,32g6.,32f6.,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,2e5,2c5,2d5,2p,2c5,2a,2g#,2b,2e5,2c5,2d5,2p,c5,e5,2a5,1g#5,p";
const char mainMelodyString[] = "Tetris:d=4,o=5,b=120:e6,8b,8c6,d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,e6,8b,8c6,d6,8c6,32b.,32c6.,32b.,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,32f6.,32g6.,32f6.,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,2e6,2c6,2d6,2b,2c6,2a,1g#,32p,e6,8b,8c6,d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,e6,8b,8c6,d6,8c6,32b.,32c6.,32b.,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,32f6.,32g6.,32f6.,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,2e6,2c6,2d6,2b,2c6,2a,2g#,2b,2e6,2c6,2d6,2b,c6,e6,2a6,2g#6,p";
const char gameOverMelodyString[] = "gameover:d=4,o=4,b=200:8c5,4p,8g4,4p,4e4,6a4,6b4,6a4,6g#4,6a#4,6g#4,4g4,4f4,2g4";
const char lineEffectString[] = "line:d=4,o=5,b=140:16d6,8d6,16f#6,8a6";


void MusicManager::init()
{
  mainMelody = MelodyFactory.loadRtttlString(mainMelodyString);
  gameOverMelody = MelodyFactory.loadRtttlString(gameOverMelodyString);
  lineEffect = MelodyFactory.loadRtttlString(lineEffectString);
  player->setVolume(DEFAULT_VOLUME);
  effectPlayer->setVolume(DEFAULT_VOLUME);
}

void MusicManager::setVolume(byte volume)
{
  player->setVolume(volume);
  effectPlayer->setVolume(volume);
}

void MusicManager::unmute()
{
  player->unmute();
}

void stopMelodyCallback()
{
  MusicManager::getMusicManager()->unmute();
}

void MusicManager::startMelody()
{
  player->playAsync(mainMelody,true);
}

void MusicManager::stopMelody()
{
  player->stop();
}

void MusicManager::playLineSound(int numLines)
{
  player->mute();
  effectPlayer->playAsync(lineEffect,false,stopMelodyCallback);
}

void MusicManager::playGameOverSound()
{
  player->playAsync(gameOverMelody);
}

void MusicManager::increaseTempo(int increment)
{
  tempo+=(increment*TEMPO_INCREMENT);
  player->changeTempo(tempo);
}




