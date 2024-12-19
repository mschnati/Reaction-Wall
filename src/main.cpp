#include "globals.h"
#include "animations.h"
#include "matrix.h"
#include "games/reactiongame.h"
#include "games/tictactoe.h"

#define DISPLAY_TIME 1000
#define NEXT_ANIMATION_BUTTON 6
#define PREV_ANIMATION_BUTTON 4
#define PLAY_REACTION_BUTTON 5
#define PLAY_TICTACTOE_BUTTON 8

enum Animation {
  SNAKE_ANIMATION = 0,
  RANDOM_BLOCK,
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
Animation currentAnimation = RANDOM_BLOCK;


void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
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

  // Display FHMS logo until any button is pressed
  display_logo();
  while (true) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
      if (checkButton(i)) {
        return;
      }
    }
  }
}

bool gameIsRunning() {
    return reaction_game_is_running(&gameState) || tictactoe_is_running(&tictactoeState);
}

void loop() {
  // Start reaction game when middle button is pressed
  if (!gameIsRunning() && checkButton(PLAY_REACTION_BUTTON)) {
      FastLED.clear();
      FastLED.show();
      reaction_game_start(&gameState);
  }

  // Start tictactoe game when top middle button is pressed
  if (!gameIsRunning() && checkButton(PLAY_TICTACTOE_BUTTON)) {
      FastLED.clear();
      FastLED.show();
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
        default:
            break;
    }
  }
}

#ifdef TEST
void buttonTest() {

  Serial.begin(9600);  // Add debug output
  unsigned long lastPrintTime = 0;
  const unsigned long PRINT_INTERVAL = 1000; // 1 second between prints
  int readings[] = {digitalRead(BUTTON_1), digitalRead(BUTTON_2), digitalRead(BUTTON_3), 
                    digitalRead(BUTTON_4), digitalRead(BUTTON_5), digitalRead(BUTTON_6), 
                    digitalRead(BUTTON_7), digitalRead(BUTTON_8), digitalRead(BUTTON_9)};
  
    if (millis() - lastPrintTime >= PRINT_INTERVAL) {
        for (int i = 0; i < 9; i++) {
            if (readings[i] != lastButtonStates[i]) {
                Serial.print("Button ");
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.println(readings[i] ? "HIGH" : "LOW");
            }
        }
        lastPrintTime = millis();
    }

  for (int i = 0; i < 9; i++) {
    if (readings[i] != lastButtonStates[i]) {
      lastDebounceTime[i] = millis();
      Serial.print("Button ");
      Serial.print(i+1);
      Serial.println(" state changed");
    }

    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_TIME) {
      if (readings[i] != buttonStates[i]) {
        buttonStates[i] = readings[i];
        if (buttonStates[i] == LOW) {
          // set to random color
          setBlockColor_3(i / 3, i % 3, CRGB(random(255), random(255), random(255)));
          FastLED.show();
          delay(DISPLAY_TIME);
          setBlockColor_3(i / 3, i % 3, CRGB::Black);
          FastLED.show();
        }
      }
    }
    lastButtonStates[i] = readings[i];
  }
}
  #endif


