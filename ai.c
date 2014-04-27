/*
 ============================================================================
 Name        : ai.c
 Author      : Eduardo Vieira
 Version     :
 Copyright   : Licensed under the beer license
 Description : Tic Tac Toe Game
 Strategy    : http://www.cs.berkeley.edu/~bh/v1ch6/ttt.html
 ============================================================================
 */

#include <stdio.h>
#include "tictactoe.h"
#include "ai.h"

int get_free_column(int game[3][3], int line)
{
	int c;

	for (c = 0; c < 3; c++) {
		if (game[line][c] == EMPTY)
			return c;
	}

	return -1;
}

int get_free_line(int game[3][3], int column)
{
	int l;

	for (l = 0; l < 3; l++) {
		if (game[l][column] == EMPTY)
			return l;
	}

	return -1;
}

st_move cpu_movement (int game[3][3])
{
	int l, c;
	st_move move = { .line = -1, .column = -1 };
	
	/* Win: If you have two in a row, you can place a third to get three in a row. */
	printf("Win\n");
	for (l = 0; l < 3; l++) {
		int count = 0;
		for (c = 0; c < 3; c++) {
			if (game[l][c] == CIRCLE) {
				if (++count == 2) {
					move.line = l;
					move.column = get_free_column(game, l);
					if (move.column >= 0)
						return move;
				}
			}
		}
	}

	for (c = 0; c < 3; c++) {
		int count = 0;
		for (l = 0; l < 3; l++) {
			if (game[l][c] == CIRCLE) {
				if (++count == 2) {
					move.column = c;
					move.line = get_free_line(game, c);
					if (move.line >= 0)
						return move;
				}
			}
		}
	}

	if (game[0][0] == CIRCLE && game[1][1] == CIRCLE && game[2][2] == EMPTY) {
		move.line = move.column = 2;
		return move;
	}
	else if (game[1][1] == CIRCLE && game[2][2] == CIRCLE && game[0][0] == EMPTY) {
		move.line = move.column = 0;
		return move;
	}
	else if (game[2][0] == CIRCLE && game[1][1] == CIRCLE && game[0][2] == EMPTY) {
		move.line = 0;
		move.column = 2;
		return move;
	}
	else if (game[0][2] == CIRCLE && game[1][1] == CIRCLE && game[2][0] == EMPTY) {
		move.line = 2;
		move.column = 0;
		return move;
	}
	else if (game[1][1] == EMPTY && ((game[0][0] == CIRCLE && game[2][2] == CIRCLE) || (game[2][0] == CIRCLE && game[0][2] == CIRCLE))) {
		move.line = move.column = 1;
		return move;
	}

	/* Block: If the opponent has two in a row, you must play the third to block the opponent. */
	printf("Block\n");
	for (l = 0; l < 3; l++) {
		int count = 0;
		for (c = 0; c < 3; c++) {
			if (game[l][c] == CROSS) {
				if (++count == 2) {
					move.line = l;
					move.column = get_free_column(game, l);
					if (move.column >= 0)
						return move;
				}
			}
		}
	}

	for (c = 0; c < 3; c++) {
		int count = 0;
		for (l = 0; l < 3; l++) {
			if (game[l][c] == CROSS) {
				if (++count == 2) {
					move.column = c;
					move.line = get_free_line(game, c);
					if (move.line >= 0)
						return move;
				}
			}
		}
	}

	if (game[0][0] == CROSS && game[1][1] == CROSS && game[2][2] == EMPTY) {
		move.line = move.column = 2;
		return move;
	}
	else if (game[1][1] == CROSS && game[2][2] == CROSS && game[0][0] == EMPTY) {
		move.line = move.column = 0;
		return move;
	}
	else if (game[2][0] == CROSS && game[1][1] == CROSS && game[0][2] == EMPTY) {
		move.line = 0;
		move.column = 2;
		return move;
	}
	else if (game[0][2] == CROSS && game[1][1] == CROSS && game[2][0] == EMPTY) {
		move.line = 2;
		move.column = 0;
		return move;
	}
	else if (game[1][1] == EMPTY && ((game[0][0] == CROSS && game[2][2] == CROSS) || (game[2][0] == CROSS && game[0][2] == CROSS))) {
		move.line = move.column = 1;
		return move;
	}

	/* Fork: Create an opportunity where you have two threats to win (two non-blocked lines of 2). */
	printf("Fork\n");

	/* Blocking an opponent's fork: If there is a configuration where the opponent can fork, you must block that fork. */
	printf("Block fork\n");

	/* Center: You play the center if open. */
	printf("Center\n");
	if (game[1][1] == EMPTY) {
		move.line = move.column = 1;
		return move;
	}

	/* Opposite corner:  If the opponent is in the corner, you play the opposite corner. */
	printf("Opposite corner\n");
	if (game[0][0] == CROSS && game[2][2] == EMPTY) {
		move.line = move.column = 2;
		return move;
	}
	else if (game[2][2] == CROSS && game[0][0] == EMPTY) {
		move.line = move.column = 0;
		return move;
	}
	else if (game[0][2] == CROSS && game[2][0] == EMPTY) {
		move.line = 2;
		move.column = 0;
		return move;
	}
	else if (game[0][2] == EMPTY && game[2][0] == CROSS) {
		move.line = 0;
		move.column = 2;
		return move;
	}

	/* Empty corner:  You play in a corner square. */
	printf("Empty corner\n");
	if (game[0][0] == EMPTY) {
		move.line = move.column = 0;
		return move;
	}
	else if (game[2][2] == EMPTY) {
		move.line = move.column = 2;
		return move;
	}
	else if (game[0][2] == EMPTY) {
		move.line = 0;
		move.column = 2;
		return move;
	}
	else if (game[2][0] == EMPTY) {
		move.line = 2;
		move.column = 0;
		return move;
	}

	/* Empty side:  You play in a middle square on any of the 4 sides. */
	printf("Empty side\n");
	if (game[0][1] == EMPTY) {
		move.line = 0;
		move.column = 1;
		return move;
	}
	else if (game[1][0] == EMPTY) {
		move.line = 1;
		move.column = 0;
		return move;
	}
	else if (game[2][1] == EMPTY) {
		move.line = 2;
		move.column = 1;
		return move;
	}
	else if (game[1][2] == EMPTY) {
		move.line = 1;
		move.column = 2;
		return move;
	}

	printf("shit!\n");
	return move;
}