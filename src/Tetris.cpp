#include <Tetris.h>

#include <Arduino.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <RandomGenerator.h>
#include <Board.h>
#include <BgEffectManager.h>

#define TARGET_FRAME_TIME     15  // Desired update rate, though if too many leds it will just run as fast as it can!
#define INITIAL_DROP_FRAMES   20  // Start of game block drop delay in frames
#define COMMAND_REPEAT_DELAY  150

#define COMPLETED_LINE_DISPLAY_TIME   15 // 15 Frames

#define GAME_OVER_WAIT_TIME   3000  // Wait for 5 sec before starting an new game

FastLED_NeoMatrix *matrix;
TextManager* tetrisTextManager;
MusicManager* tetrisMusicManager;
Settings * tetrisSettings;
Board* board; 
BgEffectManager* bgEffectManager;
MultiPlayer* tetrisMultiPlayer;
Esp32GamepadHost* tetrisGamepadHost;

bool playerOne = true;
bool inviteReceived = false;

int completedLinesDisplayCounter = 0;
bool hasCompletedLines = false;
int tetrominoeFallCountdown = 0;

unsigned char message[256];

byte dropDelay;
enum  GameState { WAIT_START = 0, PLAYING_SINGLE, WAIT_JOIN, ASK_JOIN, MULTI_COUNT_DOWN, PLAYING_MULTI, GAME_OVER };
GameState gameState = WAIT_START;
bool nextBlock;
int totalLines;
int level;
int highScore = 0, lastScore;

unsigned long loopDelayMS, lastLoop, lastRotateCommand, lastLeftCommand, lastRightCommand, lastPlusMinusCommand, gameOverTime;

uint8_t brightness;
uint8_t volume;

void showWelcomeMessage()
{
  sprintf((char *)message, "AWTRIS SCORE %u HIGH %u - PRESS ANY BUTTON TO START", lastScore, highScore);
  board->setDim(true);
  tetrisTextManager->showText(2,0,String((const char *)message),TETROMINOE_COLORS[7]);
}

void showGameOverMessage(bool highScore)
{
  if(highScore)
  {
    sprintf((char *)message, "GAME OVER NEW HIGH SCORE %u",  lastScore);
  }
  else
  {
    sprintf((char *)message, "GAME OVER SCORE %u", lastScore);
  }
  board->setDim(true);
  tetrisTextManager->showText(2,0,String((const char *)message),TETROMINOE_COLORS[7]);
  gameOverTime = millis();
}

void showYouLoseMessage(int otherScore)
{
  sprintf((char *)message, "YOU LOSE! SCORE %u vs %u", lastScore, otherScore);
  board->setDim(true);
  tetrisTextManager->showText(2,0,String((const char *)message),TETROMINOE_COLORS[7]);
  gameOverTime = millis();
}

void showYouWinMessage(int otherScore)
{
  sprintf((char *)message, "YOU WIN! SCORE %u vs %u", lastScore, otherScore);
  board->setDim(true);
  tetrisTextManager->showText(2,0,String((const char *)message),TETROMINOE_COLORS[7]);
  gameOverTime = millis();
}

void showWaitPlayer2Message()
{
  sprintf((char *)message, "WAITING FOR PLAYER 2");
  tetrisTextManager->showText(2,0,String((const char *)message),TETROMINOE_COLORS[7]);
}

void showAskJoinMessage()
{
  sprintf((char *)message, "START MULTIPLAYER GAME?");
  tetrisTextManager->showText(2,0,String((const char *)message),TETROMINOE_COLORS[7]);
}

void startNewGame(bool music)
{
    lastScore = 0;
    totalLines = 0;
    level = 0;
    dropDelay = INITIAL_DROP_FRAMES;
    nextBlock = true;
    inviteReceived = false;
    board->clearBoard();
    music ? tetrisMusicManager->startMelody() : tetrisMusicManager->stopMelody();
    tetrisTextManager->hideText();
}

void flashCountdownCallback(int count)
{ // Called each time a new number is displayed during countdown
  if(count == 0)
  { // Finished
    gameState = PLAYING_MULTI;
    startNewGame(playerOne);
  }
  else if(count == 1)
  { // Last
    tetrisMusicManager->playCountDownSound(true);
  }
  else
  { // Other
    tetrisMusicManager->playCountDownSound(false);
  }
}

void tetrisInit(FastLED_NeoMatrix * ledMatrix, TextManager * textManager, MusicManager * musicManager, Settings * settings, MultiPlayer * multiPlayer, Esp32GamepadHost* gamepadHost)
{
  matrix = ledMatrix;
  tetrisMusicManager = musicManager;
  tetrisTextManager = textManager;
  tetrisTextManager->setFlashCallback(flashCountdownCallback);
  tetrisSettings = settings;
  tetrisMultiPlayer = multiPlayer;
  tetrisGamepadHost = gamepadHost;
  board = new Board();
  bgEffectManager = new BgEffectManager();
  // Restore settings
  brightness = tetrisSettings->getBrightness();
  volume = tetrisSettings->getVolume();
  highScore = tetrisSettings->getHighScore();
  bgEffectManager->setBackgroundEffect(tetrisSettings->getBackgroundEffect());

  showWelcomeMessage();

  gameState = WAIT_START;
  loopDelayMS = TARGET_FRAME_TIME;
  lastLoop = millis() - loopDelayMS;
}

void startCountDown(bool playerOneParam)
{
    gameState = MULTI_COUNT_DOWN;
    playerOne = playerOneParam;
    tetrisTextManager->flashText(2,0,"3210",TETROMINOE_COLORS[7]);
}

void setLevel(int level)
{
    tetrisTextManager->flashNumber(0,0,level,TETROMINOE_COLORS[7]);
    dropDelay = _max(1, INITIAL_DROP_FRAMES - (level * 2));
}

// Callbacks for multiplayer mode
void inviteCallback()
{
  // if(gameState == WAIT_START || WAIT_JOIN)
  { // Allow inivite at any time to make multiplayer mode easyer to reacj
    gameState = ASK_JOIN;
    inviteReceived = true;
    showAskJoinMessage();
  }
}

void joinCallback()
{
  //if(gameState == WAIT_JOIN || ASK_JOIN)
  { // Allow inivite at any time to make multiplayer mode easyer to reacj
    startCountDown(true);
  }
}

void levelCallback(int levelParam)
{
  if(levelParam > level)
  {
    level = levelParam;
    setLevel(level);
  }
}

void addLineCallback(int numLines)
{ // Only allow 1 (for 2 lines of other player),2 (for 3lines) or 4 lines for a Tetris
  if((numLines == 1) || (numLines == 2) || (numLines == 4))
  {
    tetrisMusicManager->playPenaltySound(numLines);
    board->addPenaltyLines(numLines);
    tetrisGamepadHost->setRumble(0xFF,0xFF,1000*numLines);
  } 
}

void gameOverCallback(int score)
{
  gameState = GAME_OVER;
  tetrisMusicManager->playYouWinSound();
  showYouWinMessage(score);
  tetrisMultiPlayer->sendScore(lastScore);
}

void scoreCallback(int score)
{
  showYouLoseMessage(score);
}

void tetrisLoop(GamepadCommand* command)
{
  if ((millis() - lastLoop) >= loopDelayMS)
  {
    lastLoop = millis();
    matrix->clear();
    
    if(command)
    {
      if((command->buttons[GamepadCommand::W_PLUS] || command->buttons[GamepadCommand::W_MINUS]))
      {
        if(command->buttons[GamepadCommand::W_A])
        { // Brightness change
          if(command->buttons[GamepadCommand::W_PLUS])
          {
            if(brightness < 0xFF)
            {
              brightness++;
            }
          }
          else
          {
            if(brightness > 0)
            {
              brightness--;
            }
          }
          matrix->setBrightness(brightness);
          tetrisSettings->setBrigtness(brightness);
        }
        else if(command->buttons[GamepadCommand::W_B])
        { // Volume change
          if(command->buttons[GamepadCommand::W_PLUS])
          {
            if(volume < 0xFF)
            {
              volume++;
            }
          }
          else
          {
            if(volume > 0)
            {
              volume--;
            }
          }
          tetrisMusicManager->setVolume(volume);
          tetrisSettings->setVolume(volume);
        }
        else
        { // Background change
          if(millis()-lastPlusMinusCommand >= (COMMAND_REPEAT_DELAY*2))
          {
            lastPlusMinusCommand = millis();
            if(command->buttons[GamepadCommand::W_PLUS])
            {
              bgEffectManager->nextEffect();
            }
            else
            {
              bgEffectManager->previousEffect();
            }
            tetrisSettings->setBackgroundEffect(bgEffectManager->getBackgroundEffect());
          }
        }
      }
    }

    // Render background effect
    bgEffectManager->render(matrix);

    if (gameState == WAIT_START || gameState == GAME_OVER || gameState == ASK_JOIN)
    { // Waiting for the player (+ prevent from starting a new game to soon after game over)
      if(command && (millis() > (gameOverTime + GAME_OVER_WAIT_TIME)) && command->hasCommand())
      { 
        if(command->buttons[GamepadCommand::W_HOME])
        { // Invite for multiplayer
          gameState = WAIT_JOIN;
          showWaitPlayer2Message();
        }
        else
        { // Start new game !
          if(gameState == ASK_JOIN || inviteReceived)
          {
            tetrisMultiPlayer->sendJoin();
            startCountDown(false);
          }
          else
          {
            gameState = PLAYING_SINGLE;
            startNewGame(true);
          }
          command = NULL;
        }
      }
    }
    else if(gameState == WAIT_JOIN)
    { 
      if(command && command->buttons[GamepadCommand::W_ONE])
      { // Go back to wait single player if a is pressed
        gameState = WAIT_START;
        showWelcomeMessage();
      }
      else
      { // Broadcast invite message
        tetrisMultiPlayer->broadcastInvite();
      }
    }
    else if(gameState == PLAYING_SINGLE || gameState == PLAYING_MULTI)
    { // Game started
      // Un-dim board
      board->setDim(false);
      if (hasCompletedLines)
      { // We have highlighted complete lines, delay for visual effect
        if (completedLinesDisplayCounter > 0)
        { // Wait for completed lines to be displayed
          completedLinesDisplayCounter--;
        }
        else
        { // Completed lines display finished => remove them from the board
          board->removeHighlightedLines();
          hasCompletedLines = false;
        }
      }
      else
      {
        if (board->hasTetrominoe()) // We have a current Tetrominoe
        { // Check for user input
          if (command && (command->buttons[GamepadCommand::W_ONE] || command->buttons[GamepadCommand::W_TWO]))
          {
            if(millis()-lastRotateCommand >= COMMAND_REPEAT_DELAY)
            {
              lastRotateCommand = millis();
              if(command && command->buttons[GamepadCommand::W_ONE])
              { // Rotate left
                board->rotateTetrominoeLeft();
              }
              else
              { // Rotate right
                board->rotateTetrominoeRight();
              }
            }
          }
          
          if (command && command->buttons[GamepadCommand::W_DPAD_LEFT])
          { // Go left and check if not already on the border
            if(millis()-lastLeftCommand >= COMMAND_REPEAT_DELAY)
            {
              lastLeftCommand = millis();
              board->moveTetrominoeLeft();
            }
          }
          else if (command && command->buttons[GamepadCommand::W_DPAD_RIGHT]) 
          { // Go right and check if not already on the border
            if(millis()-lastRightCommand >= COMMAND_REPEAT_DELAY)
            {
              lastRightCommand = millis();
              board->moveTetrominoeRight();
            }
          }
          if (command &&  command->buttons[GamepadCommand::W_DPAD_DOWN] ) 
          { // Go down
            tetrominoeFallCountdown = 1; // Force drop on next cycle
          }
            
          if (tetrominoeFallCountdown <= 1)
          { // It's time for the current Tetrominoe to go down
            tetrominoeFallCountdown = dropDelay;
            if(!board->moveTetrominoeDown())
            { // Tetrominoe cannot go down => send next block
                nextBlock = true;
            }
            // Reset fall down counter
            tetrominoeFallCountdown = dropDelay;
          }
        }
        if (nextBlock)
        { // Start new block
          if (board->hasTetrominoe()) // We have a current block so add to playfield before creating new block
          { // Seal current Tetrominoe to the board
            board->sealTetrominoe();
            int numlines = 0;
            // Make completed lines highlight sprite & score
            for(int i = 0; i < BOARD_HEIGHT ; i++)
            {
              if(board->isLineComplete(i))
              { 
                board->highlightLine(i);
                numlines++;
              }
            }
            lastScore += 1;
            if (numlines > 0)
            { // We have completed lines
              completedLinesDisplayCounter = COMPLETED_LINE_DISPLAY_TIME;   // Set delay for highlight display to 15 loops
              hasCompletedLines = true;
              // Sound effect
              tetrisMusicManager->playLineSound(numlines);
              // Update tempo
              tetrisMusicManager->increaseTempo(numlines);
              // Rumble
              tetrisGamepadHost->setRumble(0x7F,0x7F,250*numlines);
              if (numlines == 1)
              {
                lastScore += 4;
              }
              else if (numlines == 2)
              {
                lastScore += 12;
                if(gameState == PLAYING_MULTI) tetrisMultiPlayer->sendLine(1);
              }
              else if (numlines == 3)
              {
                lastScore += 20;
                if(gameState == PLAYING_MULTI) tetrisMultiPlayer->sendLine(2);
              }
              else if (numlines == 4)
              {
                lastScore += 40;
                if(gameState == PLAYING_MULTI) tetrisMultiPlayer->sendLine(4);
              }
              // Reduce drop delay according to new line count
              totalLines += numlines;
              if((totalLines / 10) > level)
              {
                level++;
                if(gameState == PLAYING_MULTI) tetrisMultiPlayer->sendLevel(level);
                setLevel(level);
              }
            }
          }
          // Start the new block
          deal();
          // Show next color on gamepad
          CRGB nextColor = TETROMINOE_COLORS[getNextTetrominoe()];
          tetrisGamepadHost->setLed(GamepadColor(nextColor.r,nextColor.g,nextColor.b),500);
          Tetrominoe::Type j = getCurrentTetrominoe();
          if(board->addTetrominoe(j))
          { // New block added, init drop delay
            tetrominoeFallCountdown = dropDelay;
            nextBlock = false;
          }
          else
          { // New Tetrominoe could not be added => Game over
            tetrominoeFallCountdown = 2;
            tetrisMusicManager->playGameOverSound();
            tetrisGamepadHost->setRumble(0xFF,0xFF,2000);
            if(gameState == PLAYING_MULTI)
            {
              showYouLoseMessage(0);
              tetrisMultiPlayer->sendGameOver(lastScore);
            }
            else
            { // Check high score in single player mode only
              if (lastScore > highScore)
              {
                highScore = lastScore;
                tetrisSettings->setHighScore(highScore);
                showGameOverMessage(true);
              }
              else
              {
                showGameOverMessage(false);
              }
            }
            gameState = GAME_OVER;
          }
        }
        // Update falldown counter
        tetrominoeFallCountdown--;
      }
    }
    board->render(matrix);
    // Show a hint for next Tetrominoe
    matrix->drawPixel(7,0,TETROMINOE_COLORS[getNextTetrominoe()]);
    tetrisTextManager->renderText();
    matrix->show();
  }
}