#include "globals.h"
#include "animations.h"
#define RANDOM_BLOCK_INTERVAL 1000  // 1 second between updates
#define NUM_BLOCKS 3

static unsigned long lastRandomBlockUpdate = 0;

/**
 * Lights up, up to NUM_BLOCKS random blocks with random colors.
 */
void randomBlock() {
    unsigned long currentTime = millis();
    if (currentTime - lastRandomBlockUpdate >= RANDOM_BLOCK_INTERVAL) {
        FastLED.clear();
        // Light up random blocks
        for (int i = 0; i < NUM_BLOCKS; i++) {
            int x = random(0, 3);
            int y = random(0, 3);
            CRGB color = CHSV(random(255), 255, 255);
            setBlockColor_3(x, y, color);
        }
        FastLED.show();
        lastRandomBlockUpdate = currentTime;
    }
}

#ifdef RAINBOW_SNAKE
unsigned long lastFoodTime = 0;
int foodX = -1;
int foodY = -1;
const unsigned long FOOD_INTERVAL = 5000; // 5 seconds in milliseconds

/**
 * Spawns food at a random location on the LED matrix.
 * Food is represented by a red LED.
 * @param state The current state of the snake animation.
 */
static void spawn_food(SnakeState* state) {
    do {
        state->foodX = random(0, 16);
        state->foodY = random(0, 16);
    } while (leds[LEDMatrix[state->foodX][state->foodY]] != CRGB::Black);
    leds[LEDMatrix[state->foodX][state->foodY]] = CRGB::Red;
}

/**
 * Initializes the snake animation with the given state.
 * @param state The state of the snake animation.
 * @param wrapAround Whether the snake should wrap around the LED matrix.
 */
void snake_animation_init(SnakeState* state, bool wrapAround) {
    state->x = random(0, 16);
    state->y = random(0, 16);
    state->dx = 1;
    state->dy = 0;
    state->snakeLength = 8;
    state->pathIndex = 0;
    state->lastFoodTime = millis();
    state->lastUpdate = millis();
    state->isActive = true;
    state->wrapAround = wrapAround;
    FastLED.clear();
    spawn_food(state);
}

/**
 * Updates the snake animation with the given state.
 * @param state The state of the snake animation.
 */
void snake_update(SnakeState* state) {
    if (!state->isActive) return;

    unsigned long currentTime = millis();
    if (currentTime - state->lastUpdate < 40) return;

    // Spawn food at interval if necessary
    if (currentTime - state->lastFoodTime >= FOOD_INTERVAL) {
        spawn_food(state);
        state->lastFoodTime = currentTime;
    }

    // Check if we need to spawn food after eating
    bool ateFood = false;

    // Movement logic with collision detection
    bool moved = false;
    int directions[6][2];
    int dirIndex = 0;

    // Weight current direction
    directions[dirIndex][0] = state->dx;
    directions[dirIndex][1] = state->dy;
    dirIndex++;
    directions[dirIndex][0] = state->dx;
    directions[dirIndex][1] = state->dy;
    dirIndex++;

    // Add other directions
    int possibleDirections[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    for (int k = 0; k < 4; k++) {
        int pdx = possibleDirections[k][0];
        int pdy = possibleDirections[k][1];
        if (pdx != state->dx || pdy != state->dy) {
            directions[dirIndex][0] = pdx;
            directions[dirIndex][1] = pdy;
            dirIndex++;
        }
    }

    // Shuffle directions
    for (int s = 0; s < dirIndex; s++) {
        int r = random(s, dirIndex);
        int tempDx = directions[s][0];
        int tempDy = directions[s][1];
        directions[s][0] = directions[r][0];
        directions[s][1] = directions[r][1];
        directions[r][0] = tempDx;
        directions[r][1] = tempDy;
    }

    // Try each direction
    for (int attempt = 0; attempt < dirIndex && !moved; attempt++) {
        int newDx = directions[attempt][0];
        int newDy = directions[attempt][1];
        int newX = state->x + newDx;
        int newY = state->y + newDy;

        if (state->wrapAround) {
            newX = (newX + LED_COLS) % LED_COLS;
            newY = (newY + LED_ROWS) % LED_ROWS;
        } else {
            if (newX < 0 || newX >= LED_COLS || newY < 0 || newY >= LED_ROWS) {
                continue;
            }
        }

        // Check collision with body
        bool collision = false;
        int checkLength = (state->snakeLength < 256) ? state->snakeLength : 255;
        for (int j = 0; j < checkLength; j++) {
            int idx = (state->pathIndex - j + 256) % 256;
            if (state->path[idx][0] == newX && state->path[idx][1] == newY) {
                collision = true;
                break;
            }
        }

        if (!collision) {
            state->x = newX;
            state->y = newY;
            state->dx = newDx;
            state->dy = newDy;
            moved = true;
        }
    }

    if (!moved) {
        // Reverse if stuck
        state->dx = -state->dx;
        state->dy = -state->dy;
        state->x += state->dx;
        state->y += state->dy;
        // Handle wrap around
        if (state->wrapAround) {
            state->x = (state->x + LED_COLS) % LED_COLS;
            state->y = (state->y + LED_ROWS) % LED_ROWS;
        } else {
            if (state->x < 0) state->x = 0;
            if (state->x >= LED_COLS) state->x = LED_COLS - 1;
            if (state->y < 0) state->y = 0;
            if (state->y >= LED_ROWS) state->y = LED_ROWS - 1;
        }
    }

    // Record position after moving
    state->path[state->pathIndex][0] = state->x;
    state->path[state->pathIndex][1] = state->y;

    // Check food collision
    if (state->x == state->foodX && state->y == state->foodY) {
        state->snakeLength++;
        if (state->snakeLength >= 200) state->snakeLength = 8;
        ateFood = true;
    }

    // Set current LED with rainbow effect
    leds[LEDMatrix[state->x][state->y]] = CHSV(state->pathIndex % 255, 255, 255);

    // Clear tail
    if (state->snakeLength < 256) {
        int tailIndex = (state->pathIndex - state->snakeLength + 256) % 256;
        int tailX = state->path[tailIndex][0];
        int tailY = state->path[tailIndex][1];
        leds[LEDMatrix[tailX][tailY]] = CRGB::Black;
    }

    // Update path index
    state->pathIndex = (state->pathIndex + 1) % 256;

    // Spawn new food if ate
    if (ateFood) {
        spawn_food(state);
        state->lastFoodTime = currentTime;
    }

    FastLED.show();
    state->lastUpdate = currentTime;
}
#endif

/**
 * Basic snake animation that follows the Original LED matrix indexing.
 */
void rainbow_snake_old() {
    // light up a rainbow snake of length 8, 
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(i, 255, 255);
        FastLED.show();
        delay(10);
        if (i % 8 == 0) {
        leds[i] = CRGB::Purple;
        }
        // clear leds at i - 8
        if (i >= 8) {
        leds[i - 8] = CRGB::Black;
        } else {
        leds[i + NUM_LEDS - 8] = CRGB::Black;
        }
    }
}