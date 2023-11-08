#ifndef TETRIS_H
#define TETRIS_H

#include <AwtrisConf.h>
#include <FastLED_NeoMatrix.h>
#include <GamePad.h>

void tetrisInit(FastLED_NeoMatrix * ledMatrix);
void tetrisLoop(GamePad::Command command);

#endif 