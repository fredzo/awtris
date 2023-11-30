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
        x = currentX + xo + xx;
        y = currentY + yo + yy;
        if(x>=0 && y>=0 && x < SCREEN_WIDTH && y <= SCREEN_HEIGHT)
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
    if (c >= AwtrixFont.first && c <= AwtrixFont.last && currentY <= SCREEN_HEIGHT)
    {
      GFXglyph *glyph = &AwtrixFont.glyph[c - AwtrixFont.first];
      matrixPrint(c);
      currentY += AwtrixFont.yAdvance;
    }
  }
}

void TextManager::showText(int x, int y, String text, CRGB color)
{
  mode = SCROLL;
  xPos = x;
  yPos = y + AwtrixFont.yAdvance;
  TextManager::text = text;
  TextManager::color = color;
  show = true;
  scrollCountDown = 0xFF;
  scrollPosition = 0;
  textPixels = text.length() * AwtrixFont.yAdvance;
  scrollWaitStart = millis();
}

void TextManager::flashText(int x, int y, String text, CRGB color)
{
  mode = FLASH;
  xPos = x;
  yPos = y + AwtrixFont.yAdvance;
  currentX = xPos;
  currentY = yPos;
  TextManager::text = text;
  TextManager::color = color;
  flashColor = color;
  show = true;
  flashValue = 0xFF;
  flashWaitStart = millis();
}



void TextManager::hideText()
{
  show = false;
}

void TextManager::renderText()
{
  if(show)
  {
    switch(mode)
    {
      case FLASH :
        {
          char curChar = text.charAt(curFlashCharIndex);
          matrixPrint(curChar);
          if(millis() >= (flashWaitStart + flashWait))
          { // Start fading after delay
            flashValue-=flashSpeed;
            // Dim text
            CHSV chsvColor = rgb2hsv_approximate(flashColor);
            chsvColor.v = flashValue;
            hsv2rgb_rainbow(chsvColor,color);
            if(flashValue < 0)
            { // Text is off
              flashValue = 0xFF;
              curFlashCharIndex++;
              if(curFlashCharIndex>=text.length())
              { // Stop
                show = false;
              }
              else
              { // Show next char
                scrollWaitStart = millis();
              }
            }
          }
        }
        break;
      case SCROLL:
      default:
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
        break;
    }
  }
}


