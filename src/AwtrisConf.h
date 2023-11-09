#ifndef AWTRIS_CONF_H
#define AWTRIS_CONF_H

// Pin for button
#define LEFT_BUTTON_PIN   26
#define CENTER_BUTTON_PIN 27
#define RIGHT_BUTTON_PIN  14
// Pin for Buzzer
#define BUZZER_PIN        15
// Pin for led matrix
#define MATRIX_PIN        32  // Digital IO pin connected to the NeoPixels.
// Matrix size
#define MATRIX_WIDTH      32
#define MATRIX_HEIGHT     8
// Screen size (after rotation)
#define SCREEN_WIDTH      MATRIX_HEIGHT
#define SCREEN_HEIGHT     MATRIX_WIDTH

// Default birghtness for LedMatrix
#define DEFAULT_BRIGHTNESS  40

// Uncomment to enable logs on serial out
#define SERIAL_OUT

#endif