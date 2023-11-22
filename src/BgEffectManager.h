#ifndef BG_EFFECT_MANAGER_H
#define BG_EFFECT_MANAGER_H
#include <Arduino.h>
#include <AwtrisConf.h>
#include <FastLED_NeoMatrix.h>


class BgEffectManager
{
    public :
        enum BackgroundEffect { NONE = 0, PLAIN, SWIMLANES, CHESSBOARD, STRIPES, WAVES, PLASMA };
        BgEffectManager();

        void setBackgroundEffect(BackgroundEffect effect);

        BackgroundEffect getBackgroundEffect();

        void nextEffect();

        void previousEffect();

        void render(FastLED_NeoMatrix * ledMatrix);


    private :
        // Plasma
        uint16_t plasmaTime, plasmaShift;
        BackgroundEffect currentBackgroundEffect = NONE;



};

#endif 