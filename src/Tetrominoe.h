#ifndef TETROMINOE_H
#define TETROMINOE_H
#include <Arduino.h>
#include <FastLED_NeoMatrix.h>

#define TETROMINOE_SIZE       4
#define TETROMINOE_NUM        7
#define TETROMINOE_ROTATIONS  4

const struct CRGB TETROMINOE_COLORS[] = 
{ 
    CRGB(0, 255, 255),  // I
    CRGB(0, 0, 255),    // J
    CRGB(255, 100, 0),  // L
    CRGB(255, 255, 0),  // O
    CRGB(20, 255, 20),  // S
    CRGB(255, 0, 255),  // T
    CRGB(255, 0, 0),    // Z
    CRGB(255, 255, 255) // Comleted line
};

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

        bool getShape(int x, int y);

};

#endif 