#include <Tetris.h>

#include <Arduino.h>
#include <LEDSprites.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <RandomGenerator.h>

#define TARGET_FRAME_TIME    15  // Desired update rate, though if too many leds it will just run as fast as it can!
#define INITIAL_DROP_FRAMES  20  // Start of game block drop delay in frames
#define COMMAND_REPEAT_DELAY  150

FastLED_NeoMatrix *matrix;
MusicManager* tetrisMusicManager;

const uint8_t TetrisIData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(11110000),
  // Frame 2
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(11110000),
  // Frame 4
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  B8_3BIT(10000000)
};
const uint8_t TetrisIMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11110000),
  // Frame 2
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11110000),
  // Frame 4
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000)
};
const uint8_t TetrisJData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(22200000),
  B8_3BIT(00200000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(02000000),
  B8_3BIT(02000000),
  B8_3BIT(22000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(20000000),
  B8_3BIT(22200000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(22000000),
  B8_3BIT(20000000),
  B8_3BIT(20000000)
};
const uint8_t TetrisJMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11100000),
  B8_1BIT(00100000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(01000000),
  B8_1BIT(11000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(11100000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000)
};
const uint8_t TetrisLData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(33300000),
  B8_3BIT(30000000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(33000000),
  B8_3BIT(03000000),
  B8_3BIT(03000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(00300000),
  B8_3BIT(33300000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(30000000),
  B8_3BIT(30000000),
  B8_3BIT(33000000)
};
const uint8_t TetrisLMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11100000),
  B8_1BIT(10000000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(01000000),
  B8_1BIT(01000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(00100000),
  B8_1BIT(11100000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(11000000)
};
const uint8_t TetrisOData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(44000000),
  B8_3BIT(44000000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(44000000),
  B8_3BIT(44000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(44000000),
  B8_3BIT(44000000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(44000000),
  B8_3BIT(44000000)
};
const uint8_t TetrisOMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(11000000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(11000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(11000000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(11000000)
};
const uint8_t TetrisSData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(05500000),
  B8_3BIT(55000000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(50000000),
  B8_3BIT(55000000),
  B8_3BIT(05000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(05500000),
  B8_3BIT(55000000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(50000000),
  B8_3BIT(55000000),
  B8_3BIT(05000000)
};
const uint8_t TetrisSMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(01100000),
  B8_1BIT(11000000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(11000000),
  B8_1BIT(01000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(01100000),
  B8_1BIT(11000000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(11000000),
  B8_1BIT(01000000)
};
const uint8_t TetrisTData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(66600000),
  B8_3BIT(06000000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(06000000),
  B8_3BIT(66000000),
  B8_3BIT(06000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(06000000),
  B8_3BIT(66600000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(60000000),
  B8_3BIT(66000000),
  B8_3BIT(60000000)
};
const uint8_t TetrisTMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11100000),
  B8_1BIT(01000000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(11000000),
  B8_1BIT(01000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(11100000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(11000000),
  B8_1BIT(10000000)
};
const uint8_t TetrisZData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(77000000),
  B8_3BIT(07700000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(07000000),
  B8_3BIT(77000000),
  B8_3BIT(70000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(77000000),
  B8_3BIT(07700000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(07000000),
  B8_3BIT(77000000),
  B8_3BIT(70000000)
};
const uint8_t TetrisZMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(01100000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(11000000),
  B8_1BIT(10000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(01100000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(11000000),
  B8_1BIT(10000000)
};

#define TETRIS_SPR_WIDTH  4
#define TETRIS_SPR_HEIGHT 4
const uint8_t *TetrisSprData[] = { TetrisIData, TetrisJData, TetrisLData, TetrisOData, TetrisSData, TetrisTData, TetrisZData };
const uint8_t *TetrisSprMask[] = { TetrisIMask, TetrisJMask, TetrisLMask, TetrisOMask, TetrisSMask, TetrisTMask, TetrisZMask};
const struct CRGB TetrisColours[] = { CRGB(0, 255, 255), CRGB(0, 0, 255), CRGB(255, 100, 0), CRGB(255, 255, 0), CRGB(20, 255, 20), CRGB(255, 0, 255), CRGB(255, 0, 0) };

uint8_t PlayfieldData[SCREEN_HEIGHT * ((SCREEN_WIDTH + 7) / 8) * _3BIT];
uint8_t PlayfieldMask[SCREEN_HEIGHT * ((SCREEN_WIDTH + 7) / 8) * _1BIT];
uint8_t CompletedLinesData[TETRIS_SPR_HEIGHT * ((SCREEN_WIDTH + 7) / 8) * _1BIT];
const struct CRGB CompletedLinesColour[] = { CRGB(255, 255, 255) };
cSprite Playfield, CompletedLines, CurrentBlock;
cLEDSprites* Sprites;

unsigned char AttractMsg[144], GameOverMsg[88];
char BlankMsg[32];
// cLEDText TetrisMsg;

uint8_t DropDelay;
boolean AttractMode, NextBlock;
int16_t TotalLines;
unsigned int HighScore = 0, LastScore;

uint16_t PlasmaTime, PlasmaShift;
unsigned long LoopDelayMS, LastLoop, lastRotateCommand, lastLeftCommand, lastRightCommand, lastPlusMinusCommand;

enum BackgroundEffect { NONE = 0, PLASMA = 1 };
BackgroundEffect currentBackgroundEffect = NONE;

uint8_t brightness = DEFAULT_BRIGHTNESS;

void tetrisInit(FastLED_NeoMatrix * ledMatrix, MusicManager * musicManager)
{
  matrix = ledMatrix;
  tetrisMusicManager = musicManager;
  Sprites = new cLEDSprites(matrix);
  memset(PlayfieldData, 0, sizeof(PlayfieldData));
  memset(PlayfieldMask, 0, sizeof(PlayfieldMask));
  Playfield.Setup(matrix->width(), matrix->height(), PlayfieldData, 1, _3BIT, TetrisColours, PlayfieldMask);
  Playfield.SetPositionFrameMotionOptions(0, 0, 0, 0, 0, 0, 0, 0, 0);
  Sprites->AddSprite(&Playfield);

  memset(CompletedLinesData, 0, sizeof(CompletedLinesData));
  CompletedLines.Setup(matrix->width(), TETRIS_SPR_HEIGHT, CompletedLinesData, 1, _1BIT, CompletedLinesColour, CompletedLinesData);
  CompletedLines.SetPositionFrameMotionOptions(0, 0, 0, 0, 0, 0, 0, 0, 0);

  /*TetrisMsg.SetFont(MatriseFontData);
  sprintf((char *)BlankMsg, "%.*s", _min(((leds.Height() + TetrisMsg.FontHeight()) / (TetrisMsg.FontHeight() + 1)), (int)sizeof(BlankMsg) - 1), "                              ");
  sprintf((char *)AttractMsg, "%sTETRIS%sSCORE %u%sHIGH %u%sANY BUTTON TO START%s", BlankMsg, BlankMsg, LastScore, BlankMsg, (int)HighScore, BlankMsg, BlankMsg);
  TetrisMsg.Init(&leds, TetrisMsg.FontWidth() + 1, leds.Height(), (leds.Width() - TetrisMsg.FontWidth()) / 2, 0);
  TetrisMsg.SetBackgroundMode(BACKGND_LEAVE);
  TetrisMsg.SetScrollDirection(SCROLL_UP);
  TetrisMsg.SetTextDirection(CHAR_UP);
  TetrisMsg.SetFrameRate(1);
  TetrisMsg.SetOptionsChangeMode(INSTANT_OPTIONS_MODE);
  TetrisMsg.SetText(AttractMsg, strlen((const char *)AttractMsg));*/
  AttractMode = true;
  LoopDelayMS = TARGET_FRAME_TIME;
  LastLoop = millis() - LoopDelayMS;
  PlasmaShift = (random8(0, 5) * 32) + 64;
  PlasmaTime = 0;
}

void tetrisLoop(GamePad::Command command)
{
  if ((millis() - LastLoop) >= LoopDelayMS)
  {
    LastLoop = millis();
    matrix->clear();

    
    if ( (command.plus || command.minus))
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
          int16_t r = sin16(PlasmaTime) / 256;
          int16_t h = sin16(x * r * PLASMA_X_FACTOR + PlasmaTime) + cos16(y * (-r) * PLASMA_Y_FACTOR + PlasmaTime) + sin16(y * x * (cos16(-PlasmaTime) / 256) / 2);
          matrix->drawPixel(x, y,CHSV((uint8_t)((h / 256) + 128), 255, 64));
        }
      }
      uint16_t OldPlasmaTime = PlasmaTime;
      PlasmaTime += PlasmaShift;
      if (OldPlasmaTime > PlasmaTime)
        PlasmaShift = (random8(0, 5) * 32) + 64;
    }

    if (AttractMode)
    {
      if (command.hasCommand())
      {
        AttractMode = false;
        memset(PlayfieldData, 0, sizeof(PlayfieldData));
        memset(PlayfieldMask, 0, sizeof(PlayfieldMask));
        Sprites->RemoveSprite(&CurrentBlock);
        LastScore = 0;
        TotalLines = 0;
        DropDelay = INITIAL_DROP_FRAMES;
        CurrentBlock.SetXChange(-1);
        NextBlock = true;
        command = GamePad::NO_COMMAND;
        tetrisMusicManager->startMelody();
      }
    }
    else
    {
      if (Sprites->IsSprite(&CompletedLines))  // We have highlighted complete lines, delay for visual effect
      {
        if (CompletedLines.GetXCounter() > 0)
          CompletedLines.SetXCounter(CompletedLines.GetXCounter() - 1);
        else
        {
          Sprites->RemoveSprite(&CompletedLines);
          // Remove completed lines from playfield sprite
          uint8_t *Data = PlayfieldData;
          uint8_t *Mask = PlayfieldMask;
          uint16_t Mbpl = (SCREEN_WIDTH + 7) / 8;
          uint16_t Dbpl = Mbpl * _3BIT;
          int16_t k;
          for (int16_t i=0,j=0; i<(SCREEN_HEIGHT-1)*Dbpl; i+=Dbpl,j+=Mbpl)
          {
            for (k=0; k<SCREEN_WIDTH; k+=8)
            {
              if ((uint8_t)(0xff00 >> _min(SCREEN_WIDTH - k, 8)) != Mask[j + (k / 8)])
                break;
            }
            if (k >= SCREEN_WIDTH)
            {
              //memmove(&Data[Dbpl], &Data[0], i);
              //memset(&Data[0], 0, Dbpl);
              //memmove(&Mask[Mbpl], &Mask[0], j);
              //memset(&Mask[0], 0, Mbpl);
              memmove(&Data[i], &Data[i+Dbpl], ((SCREEN_HEIGHT-1)*Dbpl)-i);
              memset(&Data[(SCREEN_HEIGHT-1)*Dbpl], 0, Dbpl);
              memmove(&Mask[j], &Mask[j+Mbpl], ((SCREEN_HEIGHT-1)*Mbpl)-j);
              memset(&Mask[(SCREEN_HEIGHT-1)*Mbpl], 0, Mbpl);
              i-=Dbpl;
              j-=Mbpl;
            }
          }
        }
      }
      else
      {
        if (CurrentBlock.GetXChange() >= 0) // We have a current block
        {
          // Check for user input
          if ( command.a || command.b)
          {
            if(millis()-lastRotateCommand >= COMMAND_REPEAT_DELAY)
            {
              lastRotateCommand = millis();
              if ((CurrentBlock.GetCurrentFrame() % 2) == 1)  // Frame 1 or 3
              {
                if (CurrentBlock.GetXChange() == 0) // I shape, vertical
                  CurrentBlock.m_X = _min(CurrentBlock.m_X, SCREEN_WIDTH - TETRIS_SPR_WIDTH);
                else if ((CurrentBlock.GetXChange() != 3) && (CurrentBlock.GetFlags() & SPRITE_EDGE_X_MAX)) // Not O shape and near border => give some space
                  --CurrentBlock.m_X;
              }
              if(command.a)
              {
                CurrentBlock.DecreaseFrame(); // Rotate to previous frame
                Sprites->DetectCollisions(&CurrentBlock);
                if (CurrentBlock.GetFlags() & SPRITE_COLLISION)
                  CurrentBlock.IncreaseFrame();
              }
              else
              {
                CurrentBlock.IncreaseFrame(); // Rotate to next frame
                Sprites->DetectCollisions(&CurrentBlock);
                if (CurrentBlock.GetFlags() & SPRITE_COLLISION)
                  CurrentBlock.DecreaseFrame();
              }
            }
          }
          
          if ( command.left && (! (CurrentBlock.GetFlags() & SPRITE_EDGE_X_MIN)) ) // Go left and check if not already on the border
          {
            if(millis()-lastLeftCommand >= COMMAND_REPEAT_DELAY)
            {
              lastLeftCommand = millis();
              CurrentBlock.m_X--;
              Sprites->DetectCollisions(&CurrentBlock);
              if (CurrentBlock.GetFlags() & SPRITE_COLLISION)
                CurrentBlock.m_X++;
            }
          }
          
          else if ( command.right && (! (CurrentBlock.GetFlags() & SPRITE_EDGE_X_MAX)) ) // Go right and check if not already on the border
          {
            if(millis()-lastRightCommand >= COMMAND_REPEAT_DELAY)
            {
              lastRightCommand = millis();
              CurrentBlock.m_X++;
              Sprites->DetectCollisions(&CurrentBlock);
              if (CurrentBlock.GetFlags() & SPRITE_COLLISION)
                CurrentBlock.m_X--;
            }
          }
          
          if ( command.down ) // Go down
          {
            CurrentBlock.SetYCounter(1); // Force y increment on next cycle
          }
            
          // Do block checks for bottom or collision
          if (CurrentBlock.GetYCounter() <= 1) // Current block has to go down
          {
            if (CurrentBlock.GetFlags() & SPRITE_EDGE_Y_MAX)
              NextBlock = true; // Block as reached the bottom => send next block
            else
            {
              ++CurrentBlock.m_Y; // Try going down and check for collision
              Sprites->DetectCollisions(&CurrentBlock);
              --CurrentBlock.m_Y;
              if (CurrentBlock.GetFlags() & SPRITE_COLLISION)
              {
                // Block has collided check for game over
                int16_t MinY = 1;
                if ((CurrentBlock.GetCurrentFrame() % 2) == 1)
                {
                  if (CurrentBlock.GetXChange() == 0) // I shape vertical
                    MinY += 2;
                  else if (CurrentBlock.GetXChange() != 3) // Not O shape + vertical
                    MinY += 1;
                }
                else if (CurrentBlock.GetXChange() == 0)  // I shape but horizontal
                    --MinY;
                if (CurrentBlock.m_Y >= MinY)
                  NextBlock = true; // No game over => send next block
                else
                {
                  // Game over
                  CurrentBlock.SetYCounter(2);  // Stop last block moving down!
                  AttractMode = true;
                  tetrisMusicManager->playGameOverSound();
                  if (LastScore > HighScore)
                  {
                    HighScore = LastScore;
                    sprintf((char *)GameOverMsg, "%sGAME OVER%sNEW HIGH SCORE %u%s", BlankMsg, BlankMsg, LastScore, BlankMsg);
                  }
                  else
                    sprintf((char *)GameOverMsg, "%sGAME OVER%sSCORE %u%s", BlankMsg, BlankMsg, LastScore, BlankMsg);
                  sprintf((char *)AttractMsg, "%sTETRIS%sSCORE %u%sHIGH %u%sANY BUTTON TO START%s", BlankMsg, BlankMsg, LastScore, BlankMsg, HighScore, BlankMsg, BlankMsg);
                  /*TetrisMsg.SetText(GameOverMsg, strlen((char *)GameOverMsg));
                  TetrisMsg.SetBackgroundMode(BACKGND_DIMMING, 0x40);*/
                }
              }
            }
          }
        }
        if (NextBlock)  // Start new block
        {
          if (CurrentBlock.GetXChange() >= 0) // We have a current block so add to playfield before creating new block
          {
            Playfield.Combine(CurrentBlock.m_X, CurrentBlock.m_Y, &CurrentBlock);
            Sprites->RemoveSprite(&CurrentBlock);
            // Make completed lines highlight sprite & score
            memset(CompletedLinesData, 0, sizeof(CompletedLinesData));
            CompletedLines.m_Y = -1;
            uint8_t *Mask = PlayfieldMask;
            uint16_t Mbpl = (SCREEN_WIDTH + 7) / 8;
            int16_t j, numlines = 0;
            for (int16_t i=(SCREEN_HEIGHT-1)*Mbpl, y=0; i>=0; i-=Mbpl,++y)
            {
              for (j=0; j<SCREEN_WIDTH; j+=8)
              {
                if ((uint8_t)(0xff00 >> _min(SCREEN_WIDTH - j, 8)) != Mask[i + (j / 8)])
                  break;
              }
              if (j >= SCREEN_WIDTH)
              {
                if (CompletedLines.m_Y == -1)
                  CompletedLines.m_Y = y;
                memset(&CompletedLinesData[((TETRIS_SPR_HEIGHT - 1) - (y - CompletedLines.m_Y)) * Mbpl], 0xff, Mbpl);
                numlines++;
              }
            }
            if (numlines > 0)
            {
              CompletedLines.SetXCounter(15);  // Set delay for highlight display to 15 loops
              Sprites->AddSprite(&CompletedLines);
            }
            LastScore += 1;
            if (numlines == 1)
              LastScore += 4;
            else if (numlines == 2)
              LastScore += 12;
            else if (numlines == 3)
              LastScore += 20;
            else if (numlines == 4)
              LastScore += 40;
            TotalLines += numlines;
            DropDelay = _max(1, INITIAL_DROP_FRAMES - (TotalLines / 5));
          }
          // Start new block
          deal();
          Tetrominoe j = getCurrentTetrominoe();
          CurrentBlock.Setup(TETRIS_SPR_WIDTH, TETRIS_SPR_WIDTH, TetrisSprData[j], 4, _3BIT, TetrisColours, TetrisSprMask[j]);
          CurrentBlock.SetPositionFrameMotionOptions((SCREEN_WIDTH/2)-1, 0, 0, 0, 0, 0, +1, DropDelay, SPRITE_DETECT_COLLISION | SPRITE_DETECT_EDGE);
          CurrentBlock.SetXChange(j);
          Sprites->AddSprite(&CurrentBlock);
          NextBlock = false;
        }
        Sprites->UpdateSprites();
      }
    }
    Sprites->RenderSprites();
    // Show a hint for next Tetrominoe
    matrix->drawPixel(7,0,TetrisColours[getNextTetrominoe()]);
    if (AttractMode)
    {
      /*if (TetrisMsg.UpdateText() == -1)
      {
        TetrisMsg.SetText(AttractMsg, strlen((char *)AttractMsg));
        TetrisMsg.SetBackgroundMode(BACKGND_LEAVE);
        Sprites->RemoveSprite(&CurrentBlock);
        memset(PlayfieldData, 0, sizeof(PlayfieldData));
        memset(PlayfieldMask, 0, sizeof(PlayfieldMask));
      }*/
    }
    matrix->show();
  }
/*  if(SerialBT.available()){
    char keyPress = (char)SerialBT.read();
    switch(keyPress) {
      case 'w':
        currentInput = ROTATE;
        break;
      case 'a':
        currentInput = LEFT;
        break;
      case 's':
        currentInput = DOWN;
        break;
      case 'd':
        currentInput = RIGHT;
        break;
    }
    Serial.println(currentInput);
  } */
}