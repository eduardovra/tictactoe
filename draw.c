/*****************************************************************************
 * draw.c
 * Author: <eduardovra@gmail.com>
 * **************************************************************************/

#include <math.h>
#include <assert.h>

#include "draw.h"

#define GRID_COLOR SDL_MapRGB(SDL_GetVideoSurface()->format, 0xff,0xff,0xff)
#define CIRCLE_COLOR SDL_MapRGB(SDL_GetVideoSurface()->format, 0xff,0,0)
#define CROSS_COLOR SDL_MapRGB(SDL_GetVideoSurface()->format, 0,0xff,0)

#define CIRCLE_SIZE 50
#define CROSS_SIZE 50

static void validate_cord (SDL_Surface * screen, Sint32 x, Sint32 y)
{
	assert(x >= 0);
	assert(x <= screen->w);
	assert(y >= 0);
	assert(y <= screen->h);
}

static void drawPixel (SDL_Surface *screen, Sint32 x, Sint32 y, Uint32 color)
{
	Uint16 *bufp;

	validate_cord(screen, x, y);

	/* Accept only 16 bits per pixel for now */
	assert(screen->format->BytesPerPixel == 2);

	bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
	*bufp = color;
}

static void drawLine (SDL_Surface *screen, Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Uint32 color)
{
	/* Bresenham's line algorithm */
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		drawPixel(screen, x0, y0, color);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

static void drawCircle (SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel)
{
	// if the first pixel in the screen is represented by (0,0) (which is in sdl)
	// remember that the beginning of the circle is not in the middle of the pixel
	// but to the left-top from it:

	double error = (double)-radius;
	double x = (double)radius -0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;

	while (x >= y)
	{
		drawPixel(surface, (int)(cx + x), (int)(cy + y), pixel);
		drawPixel(surface, (int)(cx + y), (int)(cy + x), pixel);

		if (x != 0)
		{
			drawPixel(surface, (int)(cx - x), (int)(cy + y), pixel);
			drawPixel(surface, (int)(cx + y), (int)(cy - x), pixel);
		}

		if (y != 0)
		{
			drawPixel(surface, (int)(cx + x), (int)(cy - y), pixel);
			drawPixel(surface, (int)(cx - y), (int)(cy + x), pixel);
		}

		if (x != 0 && y != 0)
		{
			drawPixel(surface, (int)(cx - x), (int)(cy - y), pixel);
			drawPixel(surface, (int)(cx - y), (int)(cy - x), pixel);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}
}

static void drawCross (SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 color)
{
	int i, x, y;

	for (i = -radius; i <= radius; i++) {
		drawPixel(surface, i + n_cx, i + n_cy, color);
	}

	for (x = -radius, y = radius; x <= radius; x++, y--) {
		drawPixel(surface, x + n_cx, y + n_cy, color);
	}
}

static void drawGrid (SDL_Surface *screen)
{
	int i, x, y;

	/* Vertical lines */
	for (i = 1; i <= 2; i++) {
		x = i * (screen->w / 3);
		drawLine(screen, x, 0, x, screen->h, GRID_COLOR);
	}

	/* Horizontal lines */
	for (i = 1; i <= 2; i++) {
		y = i * (screen->h / 3);
		drawLine(screen, 0, y, screen->w, y, GRID_COLOR);
	}
}

void draw (SDL_Surface *screen, int game[3][3])
{
	int line, column;
	int x, x_offset, y, y_offset;

	if ( SDL_MUSTLOCK(screen) ) {
		if ( SDL_LockSurface(screen) < 0 ) {
			return;
		}
	}

	/* Game grid */
	drawGrid(screen);

	/* User's movements */
	x_offset = screen->w / 6;
	y_offset = screen->h / 6;
	for (line = 0; line < 3; line++) {
		for (column = 0; column < 3; column++) {
			x = x_offset + (line * (screen->w / 3));
			y = y_offset + (column * (screen->h / 3));
			if (game[line][column] == CROSS)
				drawCross(screen, x, y, CROSS_SIZE, CROSS_COLOR);
			else if (game[line][column] == CIRCLE)
				drawCircle(screen, x, y, CIRCLE_SIZE, CIRCLE_COLOR);
		}
	}

	if ( SDL_MUSTLOCK(screen) ) {
		SDL_UnlockSurface(screen);
	}

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void drawWinner (SDL_Surface *screen, int winner)
{
	int x = screen->w / 2, y = screen->h / 2;

	if ( SDL_MUSTLOCK(screen) ) {
		if ( SDL_LockSurface(screen) < 0 ) {
			return;
		}
	}

	/* Clear screen */
	SDL_FillRect(screen, NULL, 0);
	SDL_Flip(screen);

	if (winner == CROSS)
		drawCross(screen, x, y, CROSS_SIZE * 2, CROSS_COLOR);
	else if (winner == CIRCLE)
		drawCircle(screen, x, y, CIRCLE_SIZE * 2, CIRCLE_COLOR);

	if ( SDL_MUSTLOCK(screen) ) {
		SDL_UnlockSurface(screen);
	}

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}
