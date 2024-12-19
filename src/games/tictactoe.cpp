#include "tictactoe.h"

#define GRID_COLOR CRGB::White
#define PLAYER_1_COLOR CRGB::Red
#define PLAYER_2_COLOR CRGB::Blue
#define WIN_COLOR CRGB::Green
#define WIN_DISPLAY_TIME 3000

/**
 * Draw the grid lines
 */
static void draw_grid() {
    // Draw vertical lines
    for(int i = 0; i < 16; i++) {
        leds[LEDMatrix[i][5]] = GRID_COLOR;
        leds[LEDMatrix[i][10]] = GRID_COLOR;
    }
    // Draw horizontal lines
    for(int j = 0; j < 16; j++) {
        leds[LEDMatrix[5][j]] = GRID_COLOR;
        leds[LEDMatrix[10][j]] = GRID_COLOR;
    }
}

/**
 * Draw an X or O piece at the given position
 * @param pos: 1-9 position on the board
 * @param player: 1=X, 2=O
 * @param color: color of the piece, player 1=red, player 2=blue or green if winning piece
 */
static void draw_piece(int pos, int player, CRGB color) {
    int x = SQUARES[pos - 1][0];
    int y = SQUARES[pos - 1][1];
    
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(XO_PATTERNS[player - 1][i][j]) {
                leds[LEDMatrix[x + i][y + j]] = color;
            }
        }
    }
}

/**
 * Check if the current player has won the game
 * @param state: TicTacToeState
 */
static bool check_win(TicTacToeState* state) {
    // Check rows
    for(int i = 0; i < 3; i++) {
        if(state->board[i][0] != 0 && 
            state->board[i][0] == state->board[i][1] && 
            state->board[i][0] == state->board[i][2]) {
            state->winner.positions[0] = i * 3 + 1;
            state->winner.positions[1] = i * 3 + 2;
            state->winner.positions[2] = i * 3 + 3;
            return true;
        }
    }
    
    // Check columns
    for(int i = 0; i < 3; i++) {
        if(state->board[0][i] != 0 && 
            state->board[0][i] == state->board[1][i] && 
            state->board[0][i] == state->board[2][i]) {
            state->winner.positions[0] = i + 1;
            state->winner.positions[1] = i + 4;
            state->winner.positions[2] = i + 7;
            return true;
        }
    }
    
    // Check diagonals
    if(state->board[0][0] != 0 && 
        state->board[0][0] == state->board[1][1] && 
        state->board[0][0] == state->board[2][2]) {
        state->winner.positions[0] = 1;
        state->winner.positions[1] = 5;
        state->winner.positions[2] = 9;
        return true;
    }
    
    if(state->board[0][2] != 0 && 
        state->board[0][2] == state->board[1][1] && 
        state->board[0][2] == state->board[2][0]) {
        state->winner.positions[0] = 3;
        state->winner.positions[1] = 5;
        state->winner.positions[2] = 7;
        return true;
    }
    
    return false;
}

/**
 * Check if the game is a draw
 * @param state: TicTacToeState
 */
static bool check_draw(TicTacToeState* state) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(state->board[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Initialize the TicTacToe game state
 * @param state: TicTacToeState
 */
void tictactoe_init(TicTacToeState* state) {
    memset(state->board, 0, sizeof(state->board));
    state->currentPlayer = 1;
    state->isActive = false;
    state->selectedX = 0;
    state->selectedY = 0;
    state->lastUpdate = 0;
}

/**
 * Update the TicTacToe game state
 * @param state: TicTacToeState
 */
void tictactoe_update(TicTacToeState* state) {
    if(!state->isActive) return;
    
    FastLED.clear();
    draw_grid();
    
    // Draw existing pieces with fixed colors per player
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(state->board[i][j] != 0) {
                CRGB color = (state->board[i][j] == 1) ? PLAYER_1_COLOR : PLAYER_2_COLOR;
                draw_piece(i * 3 + j + 1, state->board[i][j], color);
            }
        }
    }

    // Write current player to display
    display.clearDisplay();
    char message[16];
    sprintf(message, "Player %d", state->currentPlayer);
    updateDisplay("TicTacToe", 0, 1);
    updateDisplay(message, 3, 1);


    for(int i = 0; i < 9; i++) {
        if(checkButton(i + 1)) {
            int row = i / 3;
            int col = i % 3;
            if(state->board[row][col] == 0) {
                state->board[row][col] = state->currentPlayer;
                if(check_win(state)) {
                    unsigned long endTime = millis();
                    state->winner.hasWinner = true;
                    // Display winning pieces in green
                    for(int i = 0; i < 3; i++) {
                        draw_piece(state->winner.positions[i], state->currentPlayer, WIN_COLOR);
                    }
                    FastLED.show();
                    // Show winning player on display
                    display.clearDisplay();
                    sprintf(message, "Player %d wins!", state->currentPlayer);
                    updateDisplay("TicTacToe", 0, 1);
                    updateDisplay(message, 3, 1);

                    while (millis() - endTime < WIN_DISPLAY_TIME) {
                        // Wait to display winning pieces
                    }
                    state->isActive = false;
                    FastLED.clear();
                    return;
                } else if (check_draw(state)) {
                    // Draw last move
                    draw_piece(row * 3 + col + 1, state->currentPlayer, (state->currentPlayer == 1) ? PLAYER_1_COLOR : PLAYER_2_COLOR);
                    FastLED.show();
                    delay(1000);
                    state->isActive = false;
                    FastLED.clear();
                    // draw : between the 0s
                    leds[LEDMatrix[7][8]] = CRGB::Orange;
                    leds[LEDMatrix[9][8]] = CRGB::Orange;
                    display_digit(0, 6, 4, CRGB::Orange);
                    display_digit(0, 6, 10, CRGB::Orange);
                    delay(WIN_DISPLAY_TIME);
                    FastLED.clear();
                    return;
                }
                state->currentPlayer = (state->currentPlayer == 1) ? 2 : 1;
            }
            break;
        }
    }
    
    FastLED.show();
}

/**
 * Check if the TicTacToe game is running
 * @param state: TicTacToeState
 * @return true if the game is active
 */
bool tictactoe_is_running(TicTacToeState* state) {
    return state->isActive;
}

/**
 * Start the TicTacToe game
 * @param state: TicTacToeState
 */
void tictactoe_start(TicTacToeState* state) {
    tictactoe_init(state);
    state->isActive = true;
}

