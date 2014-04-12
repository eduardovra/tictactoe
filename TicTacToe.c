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
#include "SDL/SDL_ttf.h"

#include "draw.h"

#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		640

int Keep_Running = 1;

void input (void)
{
	SDL_Event event;

	while ( SDL_PollEvent(&event) ) {
		switch (event.type) {
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

void process (void)
{

}

int main (int argc, char *argv[])
{
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
#if 1
	if ( TTF_Init() == -1 )
	{
		fprintf(stderr, "Unable to initialize SDL_ttf: %s \n", TTF_GetError());
	}
#endif
	/* seed pseudo random numbers generator */
	srand(time(NULL));

	/* Game main loop */
	do {
		/* Draw screen */
		draw(screen);

		/* Capture user input */
		input();

		/* Process user input */
		process();
	} while (Keep_Running);

	atexit(SDL_Quit);

	return EXIT_SUCCESS;
}
