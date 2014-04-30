/*
 ============================================================================
 Name        : tictactoe.c
 Author      : Eduardo Vieira
 Version     :
 Copyright   : Licensed under the beer license
 Description : Tic Tac Toe Game
 ============================================================================
 */

#include <stdio.h>

#include <SDL.h>

#include "tictactoe.h"
#include "draw.h"
#include "ai.h"

int Winner = 0;
int Game[3][3];

/* Get a x, y coordinate and return what line and column it belongs to */
void getLineAndColumn (int *line, int *column, Uint16 x, Uint16 y)
{
	*line = y / (WINDOW_WIDTH / 3);
	*column = x / (WINDOW_HEIGHT / 3);
}

/* Capture any keyboard or mouse input */
void input (int *line, int *column)
{
	SDL_Event event;

	*line = *column = -1;

	if ( SDL_WaitEvent(&event) ) {
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				getLineAndColumn(line, column, event.button.x, event.button.y);
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_ACTIVEEVENT:
			case SDL_MOUSEMOTION:
				break;
			case SDL_QUIT:
				exit(0);
			default:
				fprintf(stderr, "Unkown event %u\n", event.type);
		}
	}
}

/* Analyse game and check if there is a winner */
int checkWinner (void)
{
	int l, c;
	int winner = -1;

	/* Check if there is no possible moves left */
	for (l = 0; l < 3; l++) {
		for (c = 0; c < 3; c++) {
			if (Game[l][c] == EMPTY)
				winner = 0;
		}
	}

	/* Horizontal */
	for (l = 0; l < 3; l++) {
		if (Game[l][0] == Game[l][1] && Game[l][0] == Game[l][2])
			winner = Game[l][0];
	}
	/* Vertical */
	for (c = 0; c < 3; c++) {
		if (Game[0][c] == Game[1][c] && Game[0][c] == Game[2][c])
			winner = Game[0][c];
	}
	/* Diagonal */
	if (Game[0][0] == Game[1][1] && Game[0][0] == Game[2][2])
		winner = Game[0][0];
	if (Game[2][0] == Game[1][1] && Game[2][0] == Game[0][2])
		winner = Game[2][0];

	return winner;
}

/* Main game logic. Takes an user move as input and generate a CPU move as reponse.
 * Determine if there is a winner.
 */
void process (int line, int column)
{
	/* If the clicked slot is already filled, do nothing */
	if (Game[line][column] != EMPTY)
		return;

	/* Negative indicates user didn't do anything */
	if (line < 0 || column < 0)
		return;

	/* Store user's movement */
	printf("User movement: line %d column %d\n", line, column);
	Game[line][column] = CROSS;

	/* Check if there is a winner */
	Winner = checkWinner();

	if (Winner != 0)
		return;

	/* Generate CPU movement */
	st_move move = cpu_movement(Game);

	/* Store computer movement */
	printf("Computer movement: line %d column %d\n", move.line, move.column);
	Game[move.line][move.column] = CIRCLE;

	/* Check if there is a winner */
	Winner = checkWinner();
}

int main (int argc, char *argv[])
{
	int line, column;
	SDL_Surface * screen;

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	/* set the title bar */
	SDL_WM_SetCaption("TicTacToe", "TicTacToe");

	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Unable to set %dx%d video: %s\n",
				WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
		exit(1);
	}

	/* Init game state */
	for (line = 0; line < 3; line ++) {
		for (column = 0; column < 3; column++) {
			Game[line][column] = EMPTY;
		}
	}

	/* Game main loop */
	do {
		/* Draw screen */
		draw(screen, Game);

		/* Capture user input */
		input(&line, &column);

		/* Process user input */
		process(line, column);
	} while (Winner == 0);

	if (Winner > 0) {
		/* Print who is the winner */
		printf("Winner: %s\n", (Winner == CROSS) ? "Cross" : "Circle");
	}
	else {
		printf("There is no winner\n");
	}

	/* Shows winner */
	drawWinner(screen, Winner);
	SDL_Delay(2000);

	atexit(SDL_Quit);

	return EXIT_SUCCESS;
}
