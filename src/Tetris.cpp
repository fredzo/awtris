#include <Tetris.h>

#include <Arduino.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <RandomGenerator.h>
#include <Board.h>

#define TARGET_FRAME_TIME    15  // Desired update rate, though if too many leds it will just run as fast as it can!
#define INITIAL_DROP_FRAMES  20  // Start of game block drop delay in frames
#define COMMAND_REPEAT_DELAY  150

#define COMPLETED_LINE_DISPLAY_TIME   15 // 15 Frames

FastLED_NeoMatrix *matrix;
TextManager* tetrisTextManager;
MusicManager* tetrisMusicManager;
Board* board; 
Settings * tetrisSettings;

int completedLinesDisplayCounter = 0;
bool hasCompletedLines = false;
int tetrominoeFallCountdown = 0;

unsigned char waitStartMessage[256], gameOverMessage[128];

byte dropDelay;
bool AttractMode;
bool nextBlock;
int totalLines;
int highScore = 0, lastScore;

uint16_t plasmaTime, plasmaShift;
unsigned long loopDelayMS, lastLoop, lastRotateCommand, lastLeftCommand, lastRightCommand, lastPlusMinusCommand;

enum BackgroundEffect { NONE = 0, PLASMA = 1 };
BackgroundEffect currentBackgroundEffect = NONE;

uint8_t brightness = DEFAULT_BRIGHTNESS;
uint8_t volume = DEFAULT_VOLUME;

void tetrisInit(FastLED_NeoMatrix * ledMatrix, TextManager * textManager, MusicManager * musicManager, Settings * settings)
{
  matrix = ledMatrix;
  tetrisMusicManager = musicManager;
  tetrisTextManager = textManager;
  tetrisSettings = settings;
  board = new Board();
  // Restore high score
  highScore = tetrisSettings->getHighScore();

  sprintf((char *)waitStartMessage, "AWTRIS SCORE %u HIGH %u - PRESS ANY BUTTON TO START", lastScore, highScore);
  board->setDim(true);
  tetrisTextManager->showText(2,0,String((const char *)waitStartMessage),TETROMINOE_COLORS[7]);


  AttractMode = true;
  loopDelayMS = TARGET_FRAME_TIME;
  lastLoop = millis() - loopDelayMS;
  plasmaShift = (random8(0, 5) * 32) + 64;
  plasmaTime = 0;
}

// Callbacks for multiplayer mode
void inviteCallback()
{ // TODO

}

void joinCallback()
{ // TODO

}

void levelCallback(int level)
{ // TODO

}

void addLineCallback(int numLines)
{ // TODO : handle game state
  // Only allow 1 (for 2 lines of other player),2 (for 3lines) or 4 lines for a Tetris
  if((numLines == 1) || (numLines == 2) || (numLines == 4))
  {
    tetrisMusicManager->playPenaltySound(numLines);
    board->addPenaltyLines(numLines);
  } 
}

void gameOverCallback(int score)
{

}

void scoreCallback(int score)
{

}


void tetrisLoop(GamePad::Command command)
{
  if ((millis() - lastLoop) >= loopDelayMS)
  {
    lastLoop = millis();
    matrix->clear();
    
    if((command.plus || command.minus))
    {
      if(command.menu)
      { // Brightness change
        if(command.plus)
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
      }
      else if(command.trig)
      { // Volume change
        if(command.plus)
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
      }
      else
      { // Background change
        if(millis()-lastPlusMinusCommand >= (COMMAND_REPEAT_DELAY*2))
        {
          lastPlusMinusCommand = millis();
          if(command.plus)
          {
            currentBackgroundEffect = (BackgroundEffect)(currentBackgroundEffect + 1);
            if(currentBackgroundEffect > PLASMA)
            {
              currentBackgroundEffect = NONE;
            }
          }
          else
          {
            currentBackgroundEffect = (BackgroundEffect)(currentBackgroundEffect - 1);
            if(currentBackgroundEffect < NONE)
            {
              currentBackgroundEffect = PLASMA;
            }
          }
        }
      }
    }

    if(currentBackgroundEffect == PLASMA)
    {
      // Fill background with dim plasma
      #define PLASMA_X_FACTOR  24
      #define PLASMA_Y_FACTOR  24
      for (int16_t x=0; x<SCREEN_WIDTH; x++)
      {
        for (int16_t y=0; y<SCREEN_HEIGHT; y++)
        {
          int16_t r = sin16(plasmaTime) / 256;
          int16_t h = sin16(x * r * PLASMA_X_FACTOR + plasmaTime) + cos16(y * (-r) * PLASMA_Y_FACTOR + plasmaTime) + sin16(y * x * (cos16(-plasmaTime) / 256) / 2);
          matrix->drawPixel(x, y,CHSV((uint8_t)((h / 256) + 128), 255, 64));
        }
      }
      uint16_t OldPlasmaTime = plasmaTime;
      plasmaTime += plasmaShift;
      if (OldPlasmaTime > plasmaTime)
        plasmaShift = (random8(0, 5) * 32) + 64;
    }

    if (AttractMode)
    { // Waiting for the player
      if (command.hasCommand())
      { // Start new game !
        AttractMode = false;
        lastScore = 0;
        totalLines = 0;
        dropDelay = INITIAL_DROP_FRAMES;
        nextBlock = true;
        command = GamePad::NO_COMMAND;
        board->clearBoard();
        tetrisMusicManager->startMelody();
        tetrisTextManager->hideText();
      }
    }
    else
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
          if ( command.a || command.b)
          {
            if(millis()-lastRotateCommand >= COMMAND_REPEAT_DELAY)
            {
              lastRotateCommand = millis();
              if(command.a)
              { // Rotate left
                board->rotateTetrominoeLeft();
              }
              else
              { // Rotate right
                board->rotateTetrominoeRight();
              }
            }
          }
          
          if (command.left)
          { // Go left and check if not already on the border
            if(millis()-lastLeftCommand >= COMMAND_REPEAT_DELAY)
            {
              lastLeftCommand = millis();
              board->moveTetrominoeLeft();
            }
          }
          else if ( command.right) 
          { // Go right and check if not already on the border
            if(millis()-lastRightCommand >= COMMAND_REPEAT_DELAY)
            {
              lastRightCommand = millis();
              board->moveTetrominoeRight();
            }
          }
          if ( command.down ) 
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
              if (numlines == 1)
                lastScore += 4;
              else if (numlines == 2)
                lastScore += 12;
              else if (numlines == 3)
                lastScore += 20;
              else if (numlines == 4)
                lastScore += 40;
              // Reduce drop delay according to new line count
              totalLines += numlines;
              dropDelay = _max(1, INITIAL_DROP_FRAMES - (totalLines / 5));
            }
          }
          // Start the new block
          deal();
          Tetrominoe::Type j = getCurrentTetrominoe();
          if(board->addTetrominoe(j))
          { // New block added, init drop delay
            tetrominoeFallCountdown = dropDelay;
            nextBlock = false;
          }
          else
          { // New Tetrominoe could not be added => Game over
            tetrominoeFallCountdown = 2;
            AttractMode = true;
            tetrisMusicManager->playGameOverSound();
            if (lastScore > highScore)
            {
              highScore = lastScore;
              tetrisSettings->setHighScore(highScore);
              tetrisSettings->save();
              sprintf((char *)gameOverMessage, "GAME OVER NEW HIGH SCORE %u",  lastScore);
            }
            else
              sprintf((char *)gameOverMessage, "GAME OVER SCORE %u", lastScore);
            //sprintf((char *)waitStartMessage, "%sTETRIS%sSCORE %u%sHIGH %u%sANY BUTTON TO START%s", BlankMsg, BlankMsg, lastScore, BlankMsg, highScore, BlankMsg, BlankMsg);
            board->setDim(true);
            tetrisTextManager->showText(2,0,String((const char *)gameOverMessage),TETROMINOE_COLORS[7]);
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
    if (AttractMode)
    {
      /*if (TetrisMsg.UpdateText() == -1)
      {
        TetrisMsg.SetText(waitStartMessage, strlen((char *)waitStartMessage));
        TetrisMsg.SetBackgroundMode(BACKGND_LEAVE);
        Sprites->RemoveSprite(&CurrentBlock);
        memset(PlayfieldData, 0, sizeof(PlayfieldData));
        memset(PlayfieldMask, 0, sizeof(PlayfieldMask));
      }*/
    }
    matrix->show();
  }
}