/*****************************************************************************
 * draw.h
 * Author: <eduardovra@gmail.com>
 * **************************************************************************/

#ifndef _DRAW_H_INCLUDED_
#define _DRAW_H_INCLUDED_

#include <SDL.h>

enum {
	EMPTY = 0,
	CIRCLE,
	CROSS,
};

void draw (SDL_Surface * screen, int game[3][3]);

void drawWinner (SDL_Surface *screen, int winner);

#endif /* _DRAW_H_INCLUDED_ */
