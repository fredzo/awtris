#include <MusicManager.h>

MusicManager* MusicManager::musicManagerInstance = nullptr;

const char mainMelodyString[] = "Tetris:d=4,o=5,b=120:e6,8b,8c6,d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,e6,8b,8c6,d6,8c6,32b.,32c6.,32b.,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,32f6.,32g6.,32f6.,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,2e6,2c6,2d6,2b,2c6,2a,1g#,32p,e6,8b,8c6,d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,e6,8b,8c6,d6,8c6,32b.,32c6.,32b.,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,32f6.,32g6.,32f6.,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,2e6,2c6,2d6,2b,2c6,2a,2g#,2b,2e6,2c6,2d6,2b,c6,e6,2a6,2g#6,p";
const char gameOverMelodyString[] = "gameover:d=4,o=4,b=200:8c5,4p,8g4,4p,4e4,6a4,6b4,6a4,6g#4,6a#4,6g#4,4g4,4f4,2g4";
const char oneLineEffectString[] = "line:d=4,o=5,b=300:32d6,32d#6,32e6,32f6,32f#6,32g6,32g#6,32a6";
const char twoThreeLinesEffectString[] = "line:d=4,o=5,b=140:16d6,8d6,16f#6,8a6";
const char tetrisEffectString[] = "line:d=4,o=5,b=120:16d6,8d6,16f#6,8a6,8f#6,2a6";
const char onePenaltyEffectString[] = "line:d=4,o=5,b=300:32a6,32g#6,32g6,32f#6,32f6,32e6,32d#6,32d6";
const char twoPenaltiesEffectString[] = "line:d=4,o=5,b=140:16a5,8a5,16f#5,8d5";
const char tetrisPenaltyEffectString[] = "line:d=4,o=5,b=120:16a5,8a5,16f#5,8d5,8f#5,2d5";


void MusicManager::init()
{
  mainMelody = MelodyFactory.loadRtttlString(mainMelodyString);
  gameOverMelody = MelodyFactory.loadRtttlString(gameOverMelodyString);
  oneLineEffect = MelodyFactory.loadRtttlString(oneLineEffectString);
  twoThreeLinesEffect = MelodyFactory.loadRtttlString(twoThreeLinesEffectString);
  tetrisEffect = MelodyFactory.loadRtttlString(tetrisEffectString);
  onePenaltyEffect = MelodyFactory.loadRtttlString(onePenaltyEffectString);
  twoPenaltiesEffect = MelodyFactory.loadRtttlString(twoPenaltiesEffectString);
  tetrisPenaltyEffect = MelodyFactory.loadRtttlString(tetrisPenaltyEffectString);
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
  tempo = INITIAL_TEMPO;
}

void MusicManager::stopMelody()
{
  player->stop();
}

void MusicManager::playLineSound(int numLines)
{
  player->mute();
  switch(numLines)
  {
    case 2 :
    case 3 :
      effectPlayer->playAsync(twoThreeLinesEffect,false,stopMelodyCallback);
      break;
    case 4 :
      effectPlayer->playAsync(tetrisEffect,false,stopMelodyCallback);
      break;
    default :
      effectPlayer->playAsync(oneLineEffect,false,stopMelodyCallback);
  }
}

void MusicManager::playPenaltySound(int numLines)
{
  player->mute();
  switch(numLines)
  {
    case 1 :
      effectPlayer->playAsync(onePenaltyEffect,false,stopMelodyCallback);
      break;
    case 2 :
      effectPlayer->playAsync(twoPenaltiesEffect,false,stopMelodyCallback);
      break;
    case 4 :
      effectPlayer->playAsync(tetrisPenaltyEffect,false,stopMelodyCallback);
      break;
    default :
      // Ignore other values
      break;
  }
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




