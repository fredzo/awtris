#include <Arduino.h>
#include <AwtrisConf.h>
#include <GamePad.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <Tetris.h>
#include <MusicManager.h>

/// GamePad instance
GamePad* gamePad;

/// Music Manager instance
MusicManager* musicManager;

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
  // GamePad init
  gamePad = new GamePad();
  gamePad->init();

  // MusicManager init
  musicManager = MusicManager::getMusicManager();
  musicManager->init();
  
  // Set leds for matrix to NEOPIXEL type with corresponding pin
  FastLED.addLeds<NEOPIXEL, MATRIX_PIN>(matrixleds, MATRIX_WIDTH * MATRIX_HEIGHT);
  neoMatrix->begin();
  // Rotate screen in vertical position for Tetris-style display
  neoMatrix->setRotation(1);
  neoMatrix->setTextWrap(false);
  neoMatrix->setBrightness(DEFAULT_BRIGHTNESS);
  neoMatrix->setTextColor(colors[0]);

  tetrisInit(neoMatrix, musicManager);
}

//////////// Boucle principale //////////////

// Button detection
boolean oldButtonState = HIGH;

int x    = 0;
int y    = 0;
int pass = 0;



void loop() {


  gamePad->processTasks();

  GamePad::Command command = gamePad->getCommand();

  // Get current button state.
  boolean newButtonState = digitalRead(CENTER_BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if((newButtonState == LOW) && (oldButtonState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newButtonState = digitalRead(CENTER_BUTTON_PIN);
    if(newButtonState == LOW) {      // Yes, still low
      if(!command.hasCommand())
      {
        command = GamePad::Command();
        command.a = true;
      }
        //Serial.println("Change special effect !");
        //effectManager->changeSpecialEffect(true);
    }
  }

    tetrisLoop(command);

    musicManager->handleMusic();


  // Set the last-read button state to the old state.
  oldButtonState = newButtonState;

}

