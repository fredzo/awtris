#ifndef BOARD_H
#define BOARD_H
#include <Arduino.h>
#include <AwtrisConf.h>
#include <FastLED_NeoMatrix.h>
#include <Tetrominoe.h>

#define BOARD_WIDTH     SCREEN_WIDTH
#define BOARD_HEIGHT    SCREEN_HEIGHT

#define ORIGIN_X        0
#define ORIGIN_Y        (SCREEN_WIDTH/2)-1


class Board
{
    enum Pixel { OFF = -1, I = 0, J, L, O, S, T, Z, LINE };
    enum Collision { NONE =0, BORDER_LEFT, BORDER_RIGHT, BOTTOM, PIXEL };
    public :
        Board();

        void clearBoard();

        void render(FastLED_NeoMatrix * ledMatrix);

        bool addTetrominoe(Tetrominoe::Type type);
        bool hasTetrominoe();
        void sealTetrominoe();

        bool rotateTetrominoeLeft();
        bool rotateTetrominoeRight();

        bool moveTetrominoeLeft();
        bool moveTetrominoeRight();
        bool moveTetrominoeDown();

        void highlightLine(int lineY);
        void removeHighlightedLines();
        bool isLineComplete(int lineY);

    private :
        Pixel pixels[SCREEN_WIDTH][SCREEN_HEIGHT];
        Tetrominoe* currentTetrominoe = NULL;

        Collision detectCollision();
        bool checkAndGiveSpace();


};

#endif 