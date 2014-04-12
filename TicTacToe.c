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
#include <time.h>

#include "SDL.h"

#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		640

int main (int argc, char *argv[]) {

	SDL_Surface * screen;

	if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	/* set the title bar */
	SDL_WM_SetCaption("Particles", "Particles");

	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Unable to set %dx%d video: %s\n",
				WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
		exit(1);
	}
#if 0
	if ( TTF_Init() == -1 )
	{
		fprintf(stderr, "Unable to initialize SDL_ttf: %s \n", TTF_GetError());
	}
#endif
	/* seed pseudo random numbers generator */
	srand(time(NULL));

	atexit(SDL_Quit);

	return EXIT_SUCCESS;
}
