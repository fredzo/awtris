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

void BgEffectManager::render(FastLED_NeoMatrix * ledMatrix)
{
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
          ledMatrix->drawPixel(x, y,CHSV((uint8_t)((h / 256) + 128), 255, 64));
        }
      }
      uint16_t OldPlasmaTime = plasmaTime;
      plasmaTime += plasmaShift;
      if (OldPlasmaTime > plasmaTime)
        plasmaShift = (random8(0, 5) * 32) + 64;
    }
}

