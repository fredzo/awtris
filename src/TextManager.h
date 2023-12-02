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

        void setFlashSpeed(byte flashSpeed) {TextManager::flashSpeed = flashSpeed;};

        void setFlashWait(unsigned long millis) {TextManager::flashWait = millis;};

        void setFlashCallback(void (*flashCallbackFunction)(int count)) {TextManager::flashCallbackFunction = flashCallbackFunction;};

        void showText(int x, int y, String text, CRGB color);

        void flashText(int x, int y, String text, CRGB color);

        void flashNumber(int x, int y, int number, CRGB color);

        void hideText();

        void renderText();

    private :
        enum Mode { SCROLL, FLASH, FLASH_NUMBER};
        Mode mode = SCROLL;
        FastLED_NeoMatrix *matrix = NULL;
        int xPos = 0;
        int yPos = 0;
        int currentX = 0;
        int currentY = 0;
        int scrollPosition = 0;
        int flashValue = 0xFF;
        int curFlashCharIndex = 0;
        int curFlashCountDown = 0;
        CRGB color;
        CRGB flashColor;
        bool show = false;
        String text;
        int scrollSpeed;
        int flashSpeed;
        int scrollCountDown;
        int textPixels;
        unsigned long scrollWait;
        unsigned long scrollWaitStart;
        unsigned long flashWait;
        unsigned long flashWaitStart;
        void (*flashCallbackFunction)(int count) = NULL;


        TextManager()
        {
        };

        ~TextManager()
        {
        };

        void matrixPrint(char c);
        void matrixPrintText();
        void matrixPrintNumber();

        static TextManager *textManagerInstance;
};

#endif 