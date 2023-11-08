#ifndef TETRIS_H
#define TETRIS_H

#include <AwtrisConf.h>
#include <FastLED_NeoMatrix.h>

// Bluetooth input
enum btnInput {NONE, ROTATE, DOWN, LEFT, RIGHT};
extern btnInput currentInput;

void tetrisInit(FastLED_NeoMatrix * ledMatrix);
void tetrisLoop();

#endif 