#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

enum Tetrominoe { I = 0, J, L, O, S, T, Z};

void deal();

Tetrominoe getCurrentTetrominoe();

Tetrominoe getNextTetrominoe();


#endif 