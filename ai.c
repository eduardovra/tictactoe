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
#include <string.h>
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

int is_diagonal (int line, int column)
{
	if (line == 1 && column == 1)
		return 1;
	else if (line == 0 && column == 0)
		return 2;
	else if (line == 2 && column == 2)
		return 2;
	else if (line == 0 && column == 2)
		return 3;
	else if (line == 2 && column == 0)
		return 3;
	else
		return 0;
}

st_move find_fork(int game[3][3], int player)
{
	int l, c;
	int l2, c2;
	int count_player = 0, count_empty = 0, forks = 0;
	int game_copy[3][3];
	st_move move = { .line = -1, .column = -1 };

	for (l = 0; l < 3; l++) {
		for (c = 0; c < 3; c++) {
			if (game[l][c] == EMPTY) {
				memcpy(game_copy, game, sizeof(game));
				game_copy[l][c] = player;

				// check fork in horizontal
				count_player = 0, count_empty = 0;

				for (c2 = 0; c2 < 3; c2++) {
					if (game_copy[l][c2] == player)
						count_player++;
					else if (game_copy[l][c2] == EMPTY)
						count_empty++;
				}

				if (count_player == 2 && count_empty == 1) {
					forks++;
				}

				// check fork in vertical
				count_player = 0, count_empty = 0;

				for (l2 = 0; l2 < 3; l2++) {
					if (game_copy[l2][c] == player)
						count_player++;
					else if (game_copy[l2][c] == EMPTY)
						count_empty++;
				}

				if (count_player == 2 && count_empty == 1) {
					forks++;
				}

				int diagonal = is_diagonal(l, c);

				if (diagonal == 1 || diagonal == 2) {
					// check fork in diagonal 1
					count_player = 0, count_empty = 0;

					for (c2 = 0, l2 = 0; c2 < 3 && l2 < 3; c2++, l2++) {
						if (game_copy[l2][c2] == EMPTY)
							count_empty++;
						else if (game_copy[l2][c2] == player)
							count_player++;
					}

					if (count_player == 2 && count_empty == 1) {
						forks++;
					}
				}
				if (diagonal == 1 || diagonal == 3) {
					// check fork in diagonal 2
					count_player = 0, count_empty = 0;

					for (c2 = 0, l2 = 2; c2 < 3 && l2 >= 0; c2++, l2--) {
						if (game_copy[l2][c2] == EMPTY)
							count_empty++;
						else if (game_copy[l2][c2] == player)
							count_player++;
					}

					if (count_player == 2 && count_empty == 1) {
						forks++;
					}
				}

				// if forks >= 2 return l, c
				if (forks >= 2) {
					move.line = l;
					move.column = c;
					return move;
				}
				else
					forks = 0;
			}
		}
	}

	return move;
}

st_move find_win (int game[3][3], int player)
{
	int l, c;
	st_move move = { .line = -1, .column = -1 };

	for (l = 0; l < 3; l++) {
		int count = 0;
		for (c = 0; c < 3; c++) {
			if (game[l][c] == player) {
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
			if (game[l][c] == player) {
				if (++count == 2) {
					move.column = c;
					move.line = get_free_line(game, c);
					if (move.line >= 0)
						return move;
				}
			}
		}
	}

	if (game[0][0] == player && game[1][1] == player && game[2][2] == EMPTY) {
		move.line = move.column = 2;
		return move;
	}
	else if (game[1][1] == player && game[2][2] == player && game[0][0] == EMPTY) {
		move.line = move.column = 0;
		return move;
	}
	else if (game[2][0] == player && game[1][1] == player && game[0][2] == EMPTY) {
		move.line = 0;
		move.column = 2;
		return move;
	}
	else if (game[0][2] == player && game[1][1] == player && game[2][0] == EMPTY) {
		move.line = 2;
		move.column = 0;
		return move;
	}
	else if (game[1][1] == EMPTY && ((game[0][0] == player && game[2][2] == player) || (game[2][0] == player && game[0][2] == player))) {
		move.line = move.column = 1;
		return move;
	}

	return move;
}

st_move cpu_movement (int game[3][3])
{
	st_move move = { .line = -1, .column = -1 };
	
	/* Win: If you have two in a row, you can place a third to get three in a row. */
	printf("Win\n");
	move = find_win(game, CIRCLE);
	if (move.line >= 0 && move.column >=0)
		return move;

	/* Block: If the opponent has two in a row, you must play the third to block the opponent. */
	printf("Block\n");
	move = find_win(game, CROSS);
	if (move.line >= 0 && move.column >=0)
		return move;

	/* Fork: Create an opportunity where you have two threats to win (two non-blocked lines of 2). */
	printf("Fork\n");
	move = find_fork(game, CIRCLE);
	if (move.line >= 0 && move.column >=0)
		return move;

	/* Blocking an opponent's fork: If there is a configuration where the opponent can fork, you must block that fork. */
	printf("Block fork\n");
	move = find_fork(game, CROSS);
	if (move.line >= 0 && move.column >=0)
		return move;

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