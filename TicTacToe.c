/*
 ============================================================================
 Name        : TicTacToe.c
 Author      : Eduardo Vieira
 Version     :
 Copyright   : Your copyright notice
 Description : Tic Tac Toe Game
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* sleep */
#include <time.h>

#include <SDL.h>
#include <SDL/SDL_ttf.h>

#include "draw.h"

#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		640

int Keep_Running = 1;
int Game[3][3];

void getLineAndColumn (int *line, int *column, Uint16 x, Uint16 y)
{
	*column = x / (WINDOW_WIDTH / 3);
	*line = y / (WINDOW_HEIGHT / 3);
	printf("Column %d Line %d\n", *column, *line);
}

void input (int *line, int *column)
{
	SDL_Event event;

	while ( SDL_PollEvent(&event) ) {
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
				Keep_Running = 0;
				break;
			default:
				fprintf(stderr, "Unkown event %u\n", event.type);
				break;
		}
	}
}

/* return a random value between [min,max] */
int get_random (int min, int max)
{
	int range = ( max - min ) + 1;
	return ((rand() / (RAND_MAX + 1.0)) * range) + min;
}

void process (int line, int column)
{
	int l, c;

	/* If the clicked slot is already filled, do nothing */
	if (Game[line][column] != EMPTY)
		return;

	/* Store user's movement */
	Game[line][column] = CROSS;

	/* Generate CPU random movement */
	do {
		l = get_random(0, 2);
		c = get_random(0, 2);
	} while (Game[l][c] != EMPTY);

	/* Store computer movement */
	Game[l][c] = CIRCLE;
}

int main (int argc, char *argv[])
{
	int line, column;
	SDL_Surface * screen;

	if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
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

	if ( TTF_Init() == -1 )
	{
		fprintf(stderr, "Unable to initialize SDL_ttf: %s \n", TTF_GetError());
	}

	/* seed pseudo random numbers generator */
	srand(time(NULL));

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
	} while (Keep_Running);

	atexit(SDL_Quit);

	return EXIT_SUCCESS;
}
