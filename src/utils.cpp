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