#ifndef UTILS_H
#define UTILS_H

#include "globals.h"

// Check for button presses
const int buttons[] = {
    BUTTON_1, BUTTON_2, BUTTON_3,
    BUTTON_4, BUTTON_5, BUTTON_6,
    BUTTON_7, BUTTON_8, BUTTON_9
};

bool checkButton(int buttonNum);

#endif