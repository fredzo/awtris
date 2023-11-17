#ifndef TETROMINOE_H
#define TETROMINOE_H
#include <Arduino.h>
#include <FastLED_NeoMatrix.h>

#define TETROMINOE_SIZE       4
#define TETROMINOE_NUM        7
#define TETROMINOE_ROTATIONS  4

class Tetrominoe
{
    public :
        enum Type { I = 0, J, L, O, S, T, Z};

        Type type;
        int x = 0;
        int y = 0;
        int rotation = 0;

        void rotateLeft();
        void rotateRight();

        void render(FastLED_NeoMatrix * ledMatrix);

        Tetrominoe(Type t) : type(t) {};

        bool** getShape();

};

#endif 