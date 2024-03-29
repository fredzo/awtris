#include <Arduino.h>
#include <AwtrisConf.h>
#include <Esp32GamepadHost.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <Tetris.h>
#include <MusicManager.h>
#include <WifiManager.h>
#include <MultiPlayer.h>
#include <Settings.h>

/// Gamepad host
Esp32GamepadHost* gamepadHost;

/// Text Manager instance
TextManager* textManager;

/// Music Manager instance
MusicManager* musicManager;

/// MultiPlayer instance
MultiPlayer* multiPlayer;

/// Settings
Settings * settings;

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
  Serial.print("Chip Model : ");
  Serial.println(ESP.getChipModel());
  Serial.print("Chip Revision : ");
  Serial.println(ESP.getChipRevision());
  Serial.print("Chip Cores : ");
  Serial.println(ESP.getChipCores());
  Serial.print("Chip freq : ");
  Serial.println(ESP.getCpuFreqMHz());
  Serial.print("Flash : ");
  Serial.println(ESP.getFlashChipSize());
#endif
  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  // Turn buzzer off
  digitalWrite(BUZZER_PIN,0);

  // Buttons
  pinMode(CENTER_BUTTON_PIN, INPUT_PULLUP);

  // Wifi init
#ifdef WIFI
  wifiManagerStart();
#endif

  // Multiplayer init
  multiPlayer = MultiPlayer::getMultiPlayer();
  multiPlayer->init();

  // Settings init
  settings = Settings::getSettings();
  settings->init();

  // Init gamepad host
  Config config = Esp32GamepadHost::createDefaultConfig();
  config.filterAccel=true;
  config.filterTouchpad=true;
  config.maxGamepads = 1;
  gamepadHost = Esp32GamepadHost::getEsp32GamepadHost();
  gamepadHost->init(config);

  // MusicManager init
  musicManager = MusicManager::getMusicManager();
  musicManager->init();
  musicManager->setVolume(settings->getVolume());

  // Set leds for matrix to NEOPIXEL type with corresponding pin
  FastLED.addLeds<NEOPIXEL, MATRIX_PIN>(matrixleds, MATRIX_WIDTH * MATRIX_HEIGHT);
  neoMatrix->begin();
  // Rotate screen in vertical position for Tetris-style display
  neoMatrix->setRotation(1);
  neoMatrix->setTextWrap(false);
  neoMatrix->setBrightness(settings->getBrightness());
  neoMatrix->setTextColor(colors[0]);

  // TextManager init
  textManager = TextManager::getTextManager();
  textManager->init(neoMatrix);
  textManager->setScrollWait(1000);
  textManager->setScroolSpeed(96);
  textManager->setFlashWait(200);
  textManager->setFlashSpeed(4);


  tetrisInit(neoMatrix, textManager, musicManager, settings, multiPlayer, gamepadHost);


}

//////////// Boucle principale //////////////

// Button detection
boolean oldButtonState = HIGH;

int x    = 0;
int y    = 0;
int pass = 0;



void loop() {


  gamepadHost->processTasks();
  GamepadCommand* command = gamepadHost->getCommand();

  // Get current button state.
  boolean newButtonState = digitalRead(CENTER_BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if((newButtonState == LOW) && (oldButtonState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newButtonState = digitalRead(CENTER_BUTTON_PIN);
    if(newButtonState == LOW) {      // Yes, still low
        //Serial.println("Change special effect !");
        //effectManager->changeSpecialEffect(true);
    }
  }

  multiPlayer->processMultiPlayer();
  tetrisLoop(command);

#ifdef WIFI
  wifiManagerHandleClient();
#endif

  // Handle setting autosave
  settings->processAutoSave();

  // Set the last-read button state to the old state.
  oldButtonState = newButtonState;

}

