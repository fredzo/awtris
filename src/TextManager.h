#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include <AwtrisConf.h>
#include <FastLED_NeoMatrix.h>


class TextManager
{
    public :


        static TextManager *getTextManager()
        {
            if (textManagerInstance == nullptr) {
                textManagerInstance = new TextManager();
            }
            return textManagerInstance;
        }

        void init(FastLED_NeoMatrix * ledMatrix);

        void setScroolSpeed(byte scrollSpeed) {TextManager::scrollSpeed = scrollSpeed;};

        void setScrollWait(unsigned long millis) {TextManager::scrollWait = millis;};

        void showText(int x, int y, String text, CRGB color);

        void hideText();

        void renderText();

    private :
        FastLED_NeoMatrix *matrix = NULL;
        int xPos = 0;
        int yPos = 0;
        int currentX = 0;
        int currentY = 0;
        int scrollPosition = 0;
        CRGB color;
        bool show = false;
        String text;
        int scrollSpeed;
        int scrollCountDown;
        int textPixels;
        unsigned long scrollWait;
        unsigned long scrollWaitStart;

        TextManager()
        {
        };

        ~TextManager()
        {
        };

        void matrixPrint(char c);
        void matrixPrintText();

        static TextManager *textManagerInstance;
};

#endif 