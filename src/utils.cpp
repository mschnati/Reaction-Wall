#include "utils.h"

static bool lastButtonStates[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
static bool buttonStates[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
static unsigned long lastDebounceTime[NUM_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0};


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