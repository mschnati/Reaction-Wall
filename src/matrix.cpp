#include "globals.h"

CRGB leds[NUM_LEDS];
int LEDMatrix[LED_ROWS][LED_COLS];

/** convert leds array to 16x16 matrix. [0][0] is bottom left corner
 *  [15][0] - [15][15]
 *     -         -
 *   [0][0] - [0][15]
 */
void ledsToMatrix() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
        if (i % 2 == 0) {
            LEDMatrix[i][j] = i * 16 + j;
        } else {
            LEDMatrix[i][j] = i * 16 + 15 - j;
        }
        }
    }
}

/**
 * Display a digit on the LED matrix.
 * @param digit The digit to display (0-9).
 * @param xOffset The x offset to start displaying the digit.
 * @param yOffset The y offset to start displaying the digit.
 * Offset is the top left corner of the digit.
 * @param color The color to display the digit.
 */
void display_digit(int digit, int xOffset, int yOffset, CRGB color) {
    if (digit < 0 || digit > 9) return;
    
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 3; col++) {
            if (DIGIT_PATTERNS[digit][4-row][col]) {  // Invert row index
                leds[LEDMatrix[row + xOffset][col + yOffset]] = color;
            } else {
                leds[LEDMatrix[row + xOffset][col + yOffset]] = CRGB::Black;
            }
        }
    }
    FastLED.show();
}

/**
 * Display a score on the LED matrix.
 * @param score The score to display.
 * @param color The color to display the score.
 */
void display_score(int score, CRGB color) {
    FastLED.clear();
    
    if (score >= 10) {
        display_digit(score / 10, 6, 4, color);  // Left digit
        display_digit(score % 10, 6, 8, color);  // Right digit
    } else {
        display_digit(score, 6, 8, color);  // Single digit, right-aligned
    }
    FastLED.show();
}

/** 
 * Set the color of a 3x3 block on the LED matrix.
 * @param x The x coordinate of the block (0-2).
 * @param y The y coordinate of the block (0-2).
 */
void setBlockColor_3(int x, int y, CRGB color) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            leds[LEDMatrix[x * 5 + i][y * 5 + j]] = color;
        }
    }
}

/**
 * Set the color of a 4x4 block on the LED matrix.
 * @param x The x coordinate of the block (0-3).
 * @param y The y coordinate of the block (0-3).
 */
void setBlockColor_4(int x, int y, CRGB color) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            leds[LEDMatrix[x * 4 + i][y * 4 + j]] = color;
        }
    }
}