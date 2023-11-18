#ifndef TETRIS_H
#define TETRIS_H

#include <AwtrisConf.h>
#include <FastLED_NeoMatrix.h>
#include <TextManager.h>
#include <GamePad.h>
#include <MusicManager.h>

void tetrisInit(FastLED_NeoMatrix * ledMatrix, TextManager * textManager, MusicManager * musicManager);
void tetrisLoop(GamePad::Command command);

#endif 