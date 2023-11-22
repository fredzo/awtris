#include <BgEffectManager.h>

BgEffectManager::BgEffectManager()
{
  plasmaShift = (random8(0, 5) * 32) + 64;
  plasmaTime = 0;
}

void BgEffectManager::setBackgroundEffect(BackgroundEffect effect)
{
    currentBackgroundEffect = effect;
}

BgEffectManager::BackgroundEffect BgEffectManager::getBackgroundEffect()
{
    return currentBackgroundEffect;
}

 
void BgEffectManager::nextEffect()
{
    currentBackgroundEffect = (BackgroundEffect)(currentBackgroundEffect + 1);
    if(currentBackgroundEffect > PLASMA)
    {
        currentBackgroundEffect = NONE;
    }
}

void BgEffectManager::previousEffect()
{
    currentBackgroundEffect = (BackgroundEffect)(currentBackgroundEffect - 1);
    if(currentBackgroundEffect < NONE)
    {
        currentBackgroundEffect = PLASMA;
    }
}

#define PLASMA_X_FACTOR  24
#define PLASMA_Y_FACTOR  24
#define STRIPES_Y_FACTOR 64
void BgEffectManager::render(FastLED_NeoMatrix * ledMatrix)
{
    uint16_t OldPlasmaTime;
    switch(currentBackgroundEffect)
    {
        case PLAIN :
            // Fill background with plain color
            for (int16_t x=0; x<SCREEN_WIDTH; x++)
            {
                for (int16_t y=0; y<SCREEN_HEIGHT; y++)
                { 
                    ledMatrix->drawPixel(x, y, CHSV(60, 60, 40));
                }
            }
            break;
        case SWIMLANES :
            // Fill background with dim stripes
            for (int16_t x=0; x<SCREEN_WIDTH; x++)
            {
                for (int16_t y=0; y<SCREEN_HEIGHT; y++)
                {   // Alternate color on each column
                    ledMatrix->drawPixel(x, y, ( (x%2) ? CHSV(60, 60, 40) : CHSV(0, 0, 64)));
                }
            }
            break;
        case CHESSBOARD :
            // Fill background with dim stripes
            for (int16_t x=0; x<SCREEN_WIDTH; x++)
            {
                for (int16_t y=0; y<SCREEN_HEIGHT; y++)
                {   
                    if(((x%2) && (y%2)) || (!(x%2) && !(y%2)))
                    {  // Chessboard
                       ledMatrix->drawPixel(x, y,CHSV(0, 0, 64));
                    }
                    else
                    {
                       ledMatrix->drawPixel(x, y,CHSV(0, 0, 32));
                    }
                }
            }
            break;
        case STRIPES :
            // Fill background with dim stripes
            for (int16_t x=0; x<SCREEN_WIDTH; x++)
            {
                for (int16_t y=0; y<SCREEN_HEIGHT; y++)
                {
                    int16_t r = sin16(plasmaTime) / 256;
                    int16_t h = x*0x1FFF + cos16(y * STRIPES_Y_FACTOR + plasmaTime);
                    ledMatrix->drawPixel(x, y,CHSV((uint8_t)((h / 256) + 128), 255, 64));
                }
            }
            OldPlasmaTime = plasmaTime;
            plasmaTime += plasmaShift;
            if (OldPlasmaTime > plasmaTime)
                plasmaShift = (random8(0, 5) * 32) + 64;
            break;
        case WAVES :
            // Fill background with dim waves
            for (int16_t x=0; x<SCREEN_WIDTH; x++)
            {
                for (int16_t y=0; y<SCREEN_HEIGHT; y++)
                {
                    int16_t r = sin16(plasmaTime) / 256;
                    int16_t h = x*0x1FFF + sin16(r * PLASMA_X_FACTOR + plasmaTime) + cos16(y * (-r) * PLASMA_Y_FACTOR + plasmaTime) + sin16(y * (cos16(-plasmaTime) / 256) / 2);
                    ledMatrix->drawPixel(x, y,CHSV((uint8_t)((h / 256) + 128), 255, 64));
                }
            }
            OldPlasmaTime = plasmaTime;
            plasmaTime += plasmaShift;
            if (OldPlasmaTime > plasmaTime)
                plasmaShift = (random8(0, 5) * 32) + 64;
            break;
        case PLASMA :
            // Fill background with dim plasma
            for (int16_t x=0; x<SCREEN_WIDTH; x++)
            {
                for (int16_t y=0; y<SCREEN_HEIGHT; y++)
                {
                int16_t r = sin16(plasmaTime) / 256;
                int16_t h = sin16(x * r * PLASMA_X_FACTOR + plasmaTime) + cos16(y * (-r) * PLASMA_Y_FACTOR + plasmaTime) + sin16(y * x * (cos16(-plasmaTime) / 256) / 2);
                ledMatrix->drawPixel(x, y,CHSV((uint8_t)((h / 256) + 128), 255, 64));
                }
            }
            OldPlasmaTime = plasmaTime;
            plasmaTime += plasmaShift;
            if (OldPlasmaTime > plasmaTime)
                plasmaShift = (random8(0, 5) * 32) + 64;
            break;
        case NONE :
        default :
            break;
    }
}

