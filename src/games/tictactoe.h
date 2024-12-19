#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "globals.h"
#include <stdbool.h>

typedef struct {
    int board[3][3];  // 0=empty, 1=X, 2=O
    int currentPlayer; // 1=X, 2=O
    bool isActive;
    int selectedX;
    int selectedY;
    unsigned long lastUpdate;
    struct {
        int positions[3];  // Store square numbers of winning line
        bool hasWinner;
    } winner;
} TicTacToeState;

const uint8_t XO_PATTERNS[2][4][4] = {
    { // X
        {1,0,0,1},
        {0,1,1,0},
        {0,1,1,0},
        {1,0,0,1}
    },
    { // O
        {0,1,1,0},
        {1,0,0,1},
        {1,0,0,1},
        {0,1,1,0}
    }
};

// Square bottom left corner coordinates for X and O
const int SQUARES[9][2] = {
    {1,1}, {1,6}, {1,11},
    {6,1}, {6,6}, {6,11},
    {11,1}, {11,6}, {11,11}
};



void tictactoe_init(TicTacToeState* state);
void tictactoe_start(TicTacToeState* state);
void tictactoe_update(TicTacToeState* state);
bool tictactoe_is_running(TicTacToeState* state);

#endif