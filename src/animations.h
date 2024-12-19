#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include "globals.h"

#ifdef RAINBOW_SNAKE
typedef struct {
    int x;
    int y;
    int dx;
    int dy;
    int snakeLength;
    int path[256][2];
    int pathIndex;
    int foodX;
    int foodY;
    unsigned long lastFoodTime;
    unsigned long lastUpdate;
    bool isActive;
    bool wrapAround;
} SnakeState;

void snake_animation_init(SnakeState* state, bool wrapAround);
void snake_update(SnakeState* state);
void rainbow_snake(bool wrapAround);
void spawnFood();
#endif

void rainbow_snake_old();
void pulse_animation(int centerX, int centerY, CRGB color);
void randomBlock();
void display_logo();

const unsigned char FHMS_LOGO [] PROGMEM = {
	0xfe, 0x7f, 0xfc, 0x7f, 0xfa, 0x9f, 0xf1, 0x0f, 0xe2, 0x17, 0xc4, 0x23, 0x88, 0x45, 0x10, 0x89, 
	0x31, 0x11, 0xca, 0x21, 0xc6, 0x43, 0xe2, 0x87, 0xf1, 0x8f, 0xf8, 0x9f, 0xfc, 0x7f, 0xfe, 0xff
};

#endif