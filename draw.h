/*****************************************************************************
 * draw.h
 * Author: <eduardovra@gmail.com>
 * **************************************************************************/

#ifndef _DRAW_H_INCLUDED_
#define _DRAW_H_INCLUDED_

#include <SDL.h>
#include <SDL/SDL_ttf.h>

enum {
	EMPTY = 0,
	CIRCLE,
	CROSS,
};

void drawPixel (SDL_Surface *screen, Sint32 x, Sint32 y, Uint32 color);

void drawLine (SDL_Surface *screen, Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Uint32 color);

void drawCircle (SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel);

void draw (SDL_Surface * screen, int game[3][3]);

#endif /* _DRAW_H_INCLUDED_ */
