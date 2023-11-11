#include <MusicManager.h>

MusicManager* MusicManager::musicManagerInstance = nullptr;

const char mainMelodyString[] = "Tetris:d=4,o=5,b=140:e6,8b,8c6,d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,e6,8b,8c6,d6,8c6,32b.,32c6.,32b.,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,32f6.,32g6.,32f6.,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,2e,2c,2d,2p,2c,1p.,e6,8b,8c6,d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,e6,8b,8c6,d6,8c6,32b.,32c6.,32b.,a,8a,8c6,e6,8d6,8c6,b.,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,32f6.,32g6.,32f6.,e6.,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,2e,2c,2d,2p,2c,p";
const char gameOverMelodyString[] = "gameover:d=4,o=4,b=200:8c5,4p,8g4,4p,4e4,6a4,6b4,6a4,6g#4,6a#4,6g#4,4g4,4f4,2g4";


void MusicManager::init()
{
  mainMelody = MelodyFactory.loadRtttlString(mainMelodyString);
  gameOverMelody = MelodyFactory.loadRtttlString(gameOverMelodyString);
  //player.playAsync(melody);

}

void MusicManager::startMelody()
{
  player->playAsync(mainMelody);
}

void MusicManager::stopMelody()
{
  player->stop();
}

void MusicManager::playLineSound()
{
  // TODO
}

void MusicManager::playGameOverSound()
{
  player->playAsync(gameOverMelody);
}

void MusicManager::handleMusic()
{
//  if(!)
}



