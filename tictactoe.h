/*
 ============================================================================
 Name        : tictactoe.h
 Author      : Eduardo Vieira
 Version     :
 Copyright   : Licensed under the beer license
 Description : Tic Tac Toe Game
 ============================================================================
 */

#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		640

#define NUM_LINES			3
#define NUM_COLUMNS			3

enum {
	EMPTY = 0,
	CIRCLE,
	CROSS,
};

typedef struct {
	int line;
	int column;
} st_move;

typedef struct {
	int board[NUM_LINES][NUM_COLUMNS];
} st_game;
