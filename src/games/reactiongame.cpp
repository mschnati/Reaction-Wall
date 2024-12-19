#include "reactiongame.h"
#include "matrix.h"

int GAME_DURATION = 15000;  // 15 seconds

/**
 * Set the duration of the reaction game in milliseconds
 * @param state Pointer to the ReactionGameState struct
 * @param duration Duration in milliseconds
 */
void reaction_game_set_duration(ReactionGameState* state, unsigned long duration) {
    GAME_DURATION = duration;
    display_score(GAME_DURATION / 1000, CRGB::YellowGreen);
}

/**
 * Initialize the reaction game state
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_init(ReactionGameState* state) {
    state->score = 0;
    state->gameStartTime = 0;
    state->activeBlock = 0;
    state->isGameActive = false;
}

/**
 * Start the reaction game
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_start(ReactionGameState* state) {
    // Set game duration until 5 pressed. 8 and 2 to increase/decrease duration
    reaction_game_set_duration(state, GAME_DURATION);
    while (!checkButton(5)) {
        if (checkButton(8)) {
            GAME_DURATION += 1000;
            reaction_game_set_duration(state, GAME_DURATION);
        } else if (checkButton(2)) {
            if (GAME_DURATION > 1000) GAME_DURATION -= 1000;
            reaction_game_set_duration(state, GAME_DURATION);
        }
        // format string with duration
        display.clearDisplay();
        updateDisplay("Reaction Game", 0, 1);
        char message[16];
        sprintf(message, "Duration: %d", GAME_DURATION / 1000);
        updateDisplay(message, 3, 1);
    }
    FastLED.clear();

    state->score = 0;
    state->gameStartTime = millis();
    state->isGameActive = true;
    reaction_game_set_new_block(state);
}

/**
 * Set a new random block
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_set_new_block(ReactionGameState* state) {
    // Clear current block
    int oldX = state->activeBlock / 3;
    int oldY = state->activeBlock % 3;
    setBlockColor_3(oldX, oldY, CRGB::Black);
    
    // Set new random block, different from the current one
    int new_block = random(9);
    while (new_block == state->activeBlock) {
        new_block = random(9);
    }
    state->activeBlock = new_block;
    int newX = state->activeBlock / 3;
    int newY = state->activeBlock % 3;
    // setBlockColor_3(newX, newY, BLOCK_COLOR);
    setBlockColor_3(newX, newY, CRGB(random(255), random(255), random(255)));
    FastLED.show();
}

/**
 * Update the reaction game state
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_update(ReactionGameState* state) {
    if (!state->isGameActive) return;

    // Check game over
    if (millis() - state->gameStartTime >= GAME_DURATION) {
        state->isGameActive = false;
        state->gameEndTime = millis(); // Record the time when the game ended

        FastLED.clear();
        FastLED.show();
        
        // Display score
        display_score(state->score, CRGB::Green);
        // Display average reaction in ms time on display
        display.clearDisplay();
        char message[32];
        sprintf(message, "Score: %d", state->score);
        updateDisplay(message, 0, 1);

        if (state->score > 0) {
            sprintf(message, "Avg Time: %d ms", GAME_DURATION / state->score);
        } else {
            sprintf(message, "No Hits");
        }
        updateDisplay(message, 3, 1);
        
        while (millis() - state->gameEndTime < 5000) {
            // Wait for 5 seconds before returning to main menu
        }
        FastLED.clear();
        FastLED.show();
        // mainMenu();
        return;
    }

    if (checkButton(state->activeBlock + 1)) {
        char message[16];
        sprintf(message, "Score: %d", state->score);
        display.clearDisplay();
        updateDisplay(message, 1, 2);
        state->score++;
        reaction_game_set_new_block(state);
    }
}

/**
 * Check if the reaction game is currently running
 * @param state Pointer to the ReactionGameState struct
 * @return True if the game is active, false otherwise
 */
bool reaction_game_is_running(ReactionGameState* state) {
    return state->isGameActive;
}

/**
 * Get the current score of the reaction game
 * @param state Pointer to the ReactionGameState struct
 * @return Current score
 */
int reaction_game_get_score(ReactionGameState* state) {
    return state->score;
}
