#ifndef GLOBALS_H
#define GLOBALS_H

/**
 * 7 8 9
 * 4 5 6
 * 1 2 3
 * Button pins 
 */
#define NUM_BUTTONS 9
#define BUTTON_1 27 // top left
#define BUTTON_2 26 // top middle
#define BUTTON_3 19 // top right
#define BUTTON_4 32 // middle left
#define BUTTON_5 18 // middle middle
#define BUTTON_6 25 // middle right
#define BUTTON_7 13 // bottom left
#define BUTTON_8 12 // bottom middle
#define BUTTON_9 14 // bottom right

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
#define BRIGHTNESS 75


extern CRGB leds[NUM_LEDS];
extern int LEDMatrix[16][16];

#endif