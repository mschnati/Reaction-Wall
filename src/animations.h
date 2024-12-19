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

#endif