#ifndef GLOBALS_H
#define GLOBALS_H

/**
 * 7 8 9
 * 4 5 6
 * 1 2 3
 * Button pins 
 */
#define NUM_BUTTONS 9
#define BUTTON_1 19 // top left
#define BUTTON_2 25 // top middle
#define BUTTON_3 26 // top right
#define BUTTON_4 18 // middle left
#define BUTTON_5 32 // middle middle
#define BUTTON_6 27 // middle right
#define BUTTON_7 14 // bottom left
#define BUTTON_8 12 // bottom middle
#define BUTTON_9 13 // bottom right

// Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SDA_PIN 21
#define SCL_PIN 22
extern Adafruit_SSD1306 display;

#include <Arduino.h>
#include <FastLED.h>
#include "config.h"
#include "matrix.h"
#include "utils.h"
#ifdef REACTION_GAME
#include "games/reactiongame.h"
#endif
#ifdef TICTACTOE_GAME
#include "games/tictactoe.h"
#endif

#define NUM_LEDS 256
#define LED_ROWS 16
#define LED_COLS 16
#define DATA_PIN 16
#define DEBOUNCE_TIME 50
#define BRIGHTNESS 40


extern CRGB leds[NUM_LEDS];
extern int LEDMatrix[16][16];

#endif