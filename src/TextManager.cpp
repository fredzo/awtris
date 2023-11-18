#include <TextManager.h>
#include <AwtrixFont.h>

TextManager* TextManager::textManagerInstance = nullptr;



void TextManager::init(FastLED_NeoMatrix * ledMatrix)
{
  matrix = ledMatrix;
}


void TextManager::matrixPrint(char c)
{
  c -= (uint8_t)pgm_read_byte(&AwtrixFont.first);
  GFXglyph *glyph = &AwtrixFont.glyph[c];
  uint8_t *bitmap = AwtrixFont.bitmap;
  uint16_t bo = glyph->bitmapOffset;
  uint8_t w = glyph->width,
          h = glyph->height;
  int8_t xo = glyph->xOffset,
         yo = glyph->yOffset;

  uint8_t xx, yy, bits = 0, bit = 0;
  int x,y;
  for (yy = 0; yy < h; yy++)
  {
    for (xx = 0; xx < w; xx++)
    {
      if (!(bit++ & 7))
      {
        bits = pgm_read_byte(&bitmap[bo++]);
      }
      if (bits & 0x80)
      {
        /*Serial.print("Draw pixel ");
        Serial.print(cursor_x + xo + xx);
        Serial.print(", ");
        Serial.println(cursor_y + yo + yy);*/
        x = currentX + xo + xx;
        y = currentY + yo + yy;
        if(x>=0 && y>=0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
        {
          matrix->drawPixel(x, y, color);
        }
      }
      bits <<= 1;
    }
  }
}

void TextManager::matrixPrintText()
{
  const char* str = text.c_str();
  while (*str)
  {
    char c = *str++;
    if (c >= AwtrixFont.first && c <= AwtrixFont.last && currentY < SCREEN_HEIGHT)
    {
      GFXglyph *glyph = &AwtrixFont.glyph[c - AwtrixFont.first];
      matrixPrint(c);
      currentY += AwtrixFont.yAdvance;
    }
  }
}

void TextManager::showText(int x, int y, String text, CRGB color)
{
  xPos = x;
  yPos = y + AwtrixFont.yAdvance;
  TextManager::text = text;
  TextManager::color = color;
  show = true;
  scrollCountDown = 0xFF;
  textPixels = text.length() * AwtrixFont.yAdvance;
  scrollWaitStart = millis();
}

void TextManager::hideText()
{
  show = false;
}

void TextManager::renderText()
{
  if(show)
  {
    currentX = xPos;
    currentY = yPos - scrollPosition;
    matrixPrintText();
    if(millis() >= (scrollWaitStart + scrollWait))
    { // Start scrolling after delay
      scrollCountDown-=scrollSpeed;
      if(scrollCountDown <=0)
      {
        scrollPosition++;
        if(scrollPosition >= textPixels)
        {
          scrollPosition = 0;
          scrollWaitStart = millis();
        }
        scrollCountDown = 0xFF;
      }
    }
  }
}


