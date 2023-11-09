#include <RandomGenerator.h>

#include <Arduino.h>

Tetrominoe bag[7];

int currentBagIndex = -1;
Tetrominoe currentTetrominoe;
Tetrominoe nextTetrominoe;

void deal()
{
  if(currentBagIndex == -1)
  { // This is the first call
    currentBagIndex = 0;
    // We need to call deal twice to init current and next tetrominoes
    deal();
  }
  if(currentBagIndex == 0)
  { // We need a new bag
    // Load the bag
    for (int i=0; i<7; i++)
    {
      bag[i] = (Tetrominoe)i;
    }
    // Shuffle it
    for (int i=0; i<7; i++)
    {
      int r = random(7);
      Tetrominoe temp = bag[i];
      bag[i] = bag[r];
      bag[r] = temp;
    }
  }
  currentTetrominoe = nextTetrominoe;
  nextTetrominoe = bag[currentBagIndex];
  currentBagIndex++;
  if(currentBagIndex >= 7)
  {
    currentBagIndex = 0;
  }
}

Tetrominoe getCurrentTetrominoe()
{
  return currentTetrominoe;
}

Tetrominoe getNextTetrominoe()
{
  return nextTetrominoe;
}
