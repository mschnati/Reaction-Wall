#include "utils.h"

static bool lastButtonStates[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
static bool buttonStates[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
static unsigned long lastDebounceTime[NUM_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * Check if a button has been pressed.
 * @param buttonNum The button number to check.
 * @return True if the button has been pressed.
 */
bool checkButton(int buttonNum) {
    int reading = digitalRead(buttons[buttonNum - 1]);
    
    if (reading != lastButtonStates[buttonNum]) {
        lastDebounceTime[buttonNum] = millis();
    }
    
    if ((millis() - lastDebounceTime[buttonNum]) > DEBOUNCE_TIME) {
        if (reading != buttonStates[buttonNum]) {
            buttonStates[buttonNum] = reading;
            if (buttonStates[buttonNum] == LOW) {
                return true;
            }
        }
    }
    
    lastButtonStates[buttonNum] = reading;
    return false;
}

/**
 * Update the display with the given message.
 * @param message The message to display.
 */
void updateDisplay(const char* message, int line, int textSize) {
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, line * 8 * textSize);
    display.println(message);
    display.display();
}

void mainMenu() {
      // Show main menu
    display.clearDisplay();
    updateDisplay("5: Reaction Game", 0, 1);
    updateDisplay("8: TicTacToe Game", 1, 1);
    updateDisplay("4/6: Change Animation", 2, 1);
}