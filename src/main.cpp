#include <Arduino.h>
#include <AwtrisConf.h>
#include <ESP32Wiimote.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <Tetris.h>


/// Wiimote
ESP32Wiimote wiimote;

static bool logging = true;
static long last_ms = 0;
static int num_run = 0, num_updates = 0;

/// Neo Matrix
#define NUMMATRIX (MATRIX_WIDTH*MATRIX_HEIGHT)

CRGB matrixleds[NUMMATRIX];
// Create a NeoMatrix of 32x8 pixels corresponding to Ulanzi Desktop Clock screen (in horizontal position)
FastLED_NeoMatrix *neoMatrix = new FastLED_NeoMatrix(matrixleds, MATRIX_WIDTH, MATRIX_HEIGHT, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);    

const uint16_t colors[] = { neoMatrix->Color(255, 0, 0), neoMatrix->Color(0, 255, 0), neoMatrix->Color(0, 0, 255) };


//////////// Initialisation du programme //////////////

void setup() {
#ifdef SERIAL_OUT
  Serial.begin(115200);
  Serial.println("Démarrage Awtrix !");
#endif
  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  // Turn buzzer off
  digitalWrite(BUZZER_PIN,0);
  // Buttons
  pinMode(CENTER_BUTTON_PIN, INPUT_PULLUP);
  // Wimote driver init
  wiimote.init();
  if (! logging)
      wiimote.addFilter(ACTION_IGNORE, FILTER_ACCEL); // optional
  
  last_ms = millis();

  // Set leds for matrix to NEOPIXEL type with corresponding pin
  FastLED.addLeds<NEOPIXEL, MATRIX_PIN>(matrixleds, MATRIX_WIDTH * MATRIX_HEIGHT);
  neoMatrix->begin();
  // Rotate screen in vertical position for Tetris-style display
  neoMatrix->setRotation(1);
  neoMatrix->setTextWrap(false);
  neoMatrix->setBrightness(40);
  neoMatrix->setTextColor(colors[0]);

  tetrisInit(neoMatrix);
}

//////////// Boucle principale //////////////

// Button detection
boolean oldButtonState = HIGH;

int x    = 0;
int y    = 0;
int pass = 0;



void loop() {


    wiimote.task();
    tetrisLoop();
    num_run++;

  // Get current button state.
  boolean newButtonState = digitalRead(CENTER_BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if((newButtonState == LOW) && (oldButtonState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newButtonState = digitalRead(CENTER_BUTTON_PIN);
    if(newButtonState == LOW) {      // Yes, still low
      currentInput = ROTATE;
        //Serial.println("Change special effect !");
        //effectManager->changeSpecialEffect(true);
    }
  }
  
  // Set the last-read button state to the old state.
  oldButtonState = newButtonState;


    if (wiimote.available() > 0) 
    {
        ButtonState  button  = wiimote.getButtonState();
        AccelState   accel   = wiimote.getAccelState();
        NunchukState nunchuk = wiimote.getNunchukState();

        num_updates++;
        if (logging)
        {
            char ca     = (button & BUTTON_A)     ? 'A' : '.';
            char cb     = (button & BUTTON_B)     ? 'B' : '.';
            char cc     = (button & BUTTON_C)     ? 'C' : '.';
            char cz     = (button & BUTTON_Z)     ? 'Z' : '.';
            char c1     = (button & BUTTON_ONE)   ? '1' : '.';
            char c2     = (button & BUTTON_TWO)   ? '2' : '.';
            char cminus = (button & BUTTON_MINUS) ? '-' : '.';
            char cplus  = (button & BUTTON_PLUS)  ? '+' : '.';
            char chome  = (button & BUTTON_HOME)  ? 'H' : '.';
            char cleft  = (button & BUTTON_LEFT)  ? '<' : '.';
            char cright = (button & BUTTON_RIGHT) ? '>' : '.';
            char cup    = (button & BUTTON_UP)    ? '^' : '.';
            char cdown  = (button & BUTTON_DOWN)  ? 'v' : '.';
      
            Serial.printf("button: %05x = ", (int)button);
            Serial.print(ca);
            Serial.print(cb);
            Serial.print(cc);
            Serial.print(cz);
            Serial.print(c1);
            Serial.print(c2);
            Serial.print(cminus);
            Serial.print(chome);
            Serial.print(cplus);
            Serial.print(cleft);
            Serial.print(cright);
            Serial.print(cup);
            Serial.print(cdown);
            Serial.printf(", wiimote.axis: %3d/%3d/%3d", accel.xAxis, accel.yAxis, accel.zAxis);
            Serial.printf(", nunchuk.axis: %3d/%3d/%3d", nunchuk.xAxis, nunchuk.yAxis, nunchuk.zAxis);
            Serial.printf(", nunchuk.stick: %3d/%3d\n", nunchuk.xStick, nunchuk.yStick);
        }
    }

    if (! logging)
    {
        long ms = millis();
        if (ms - last_ms >= 1000)
        {
            Serial.printf("Run %d times per second with %d updates\n", num_run, num_updates);
            num_run = num_updates = 0;
            last_ms += 1000;
        }
    }

/*  neoMatrix->fillScreen(0);
  neoMatrix->drawPixel(x,y,colors[pass]);
  x++;
  if(x>=SCREEN_WIDTH)
  {
    x = 0;
    y++;
    if(y>=SCREEN_HEIGHT)
    {
      y=0;
      if(++pass >= 3) pass = 0;
    }
  }
*/
/*  neoMatrix->setCursor(x, 0);
  neoMatrix->print(F("Howdy"));
  if(--x < -36) {
    x = neoMatrix->width();
    if(++pass >= 3) pass = 0;
    neoMatrix->setTextColor(colors[pass]);
  }
*/

  //neoMatrix->show();
  //delay(100);



//    delay(10);

}

