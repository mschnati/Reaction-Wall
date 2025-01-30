#include "globals.h"
#include "animations.h"
#include "matrix.h"
#include "games/reactiongame.h"
#include "games/tictactoe.h"

#define NEXT_ANIMATION_BUTTON 6
#define PREV_ANIMATION_BUTTON 4
#define PLAY_REACTION_BUTTON 5
#define PLAY_TICTACTOE_BUTTON 8

enum Animation {
  RANDOM_BLOCK = 0,
  SNAKE_ANIMATION,
  IDLE,
  WAVE,
  NUM_ANIMATIONS
};

enum Game {
  REACTION = 0,
  TICTACTOE,
  NUM_GAMES
};

ReactionGameState gameState;
SnakeState snakeState;
TicTacToeState tictactoeState;
Animation currentAnimation = SNAKE_ANIMATION;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    // Initialize I2C communication with specified SDA and SCL pins
  Wire.begin(SDA_PIN, SCL_PIN);
    // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.display();

  // Initialize LED matrix
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  int brightness = BRIGHTNESS;
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxRefreshRate(60);
  FastLED.clear();
  ledsToMatrix();

  // Set up button pins, all connected to ground
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  reaction_game_init(&gameState);
  snake_animation_init(&snakeState, false);
  tictactoe_init(&tictactoeState);

  updateDisplay("FHMS", 0, 2);
  updateDisplay("Middle to Start", 2, 1);
  updateDisplay("Up/Down adj Brightness", 3, 1);

  // Display FHMS logo until any button is pressed
  display_logo();
  unsigned long startTime = millis();
  const unsigned long waitTime = 10000;

  while (millis() - startTime < waitTime) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
      if (checkButton(i)) {
        if (i == 8) {
          brightness += 10;
          if (brightness > 255) {
            brightness = 255;
          }
          FastLED.setBrightness(brightness);
          FastLED.show();
        } else if (i == 2) {
          brightness -= 10;
          if (brightness < 0) {
            brightness = 0;
          }
          FastLED.setBrightness(brightness);
          FastLED.show();
        } else {
            // Show main menu
          display.clearDisplay();
          updateDisplay("5: Reaction Game", 0, 1);
          updateDisplay("8: TicTacToe Game", 1, 1);
          updateDisplay("4/6: Change Animation", 2, 1);
          FastLED.clear();
          return;
        }
      }
    }
  }
}

bool gameIsRunning() {
    return reaction_game_is_running(&gameState) || tictactoe_is_running(&tictactoeState);
}

void loop() {
  buttonTest();
}

void loop2() {
  // Start reaction game when middle button is pressed
  if (!gameIsRunning() && checkButton(PLAY_REACTION_BUTTON)) {
      FastLED.clear();
      FastLED.show();
      display.clearDisplay();
      updateDisplay("Reaction Game", 0, 1);
      reaction_game_start(&gameState);
  }

  // Start tictactoe game when top middle button is pressed
  if (!gameIsRunning() && checkButton(PLAY_TICTACTOE_BUTTON)) {
      FastLED.clear();
      FastLED.show();
      display.clearDisplay();
      updateDisplay("TicTacToe Game", 0, 1);
      tictactoe_start(&tictactoeState);
  }

  // Cycle through animations with buttons 4 and 6
  if (!gameIsRunning()) {
    if (checkButton(PREV_ANIMATION_BUTTON)) {
        currentAnimation = static_cast<Animation>((currentAnimation + NUM_ANIMATIONS - 1) % NUM_ANIMATIONS);
        FastLED.clear();
        FastLED.show();
    } else if (checkButton(NEXT_ANIMATION_BUTTON)) {
        currentAnimation = static_cast<Animation>((currentAnimation + 1) % NUM_ANIMATIONS);
        FastLED.clear();
        FastLED.show();
    }
  }

  // Update games
  if (tictactoe_is_running(&tictactoeState)) {
      tictactoe_update(&tictactoeState);
  } else if (reaction_game_is_running(&gameState)) {
      reaction_game_update(&gameState);
  } else {
      // Handle animations
    switch (currentAnimation) {
        case SNAKE_ANIMATION:
            snake_update(&snakeState);
            break;
        case RANDOM_BLOCK:
            randomBlock();
            break;
        case IDLE:
            idle_animation();
            break;
        case WAVE:
            wave_ripple_animation();
            break;
        default:
            break;
    }
  }
}