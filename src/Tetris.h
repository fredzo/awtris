#ifndef TETRIS_H
#define TETRIS_H

#include <AwtrisConf.h>
#include <FastLED_NeoMatrix.h>
#include <TextManager.h>
#include <GamePad.h>
#include <MusicManager.h>
#include <Settings.h>
#include <MultiPlayer.h>

void tetrisInit(FastLED_NeoMatrix * ledMatrix, TextManager * textManager, MusicManager * musicManager, Settings * settings, MultiPlayer * multiPlayer);
void tetrisLoop(GamePad::Command command);

// Callbacks for multiplayer mode
void inviteCallback();
void joinCallback();
void levelCallback(int level);
void addLineCallback(int numLines);
void gameOverCallback(int score);
void scoreCallback(int score);

#endif 