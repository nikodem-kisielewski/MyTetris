#pragma once

#include "colors.h"
#include "shapes.h"
#include "board.h"
#include "game.h"
#include "SDL.h"
#include "SDL_ttf.h"
//#include <stdio.h>
//#include <Windows.h>

#define GRID_SIZE 30
#define TOP_OFFSET 120
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 750

/*
*	Class that handles the rendering of the game.
*/
class Rendering
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect rect;
	SDL_Texture* texture;
	SDL_Surface* textSurface;
	TTF_Font* font;
	SDL_Color textColor;
public:
	Rendering();
	void drawUpdate();
	void drawShape(Shapes shape, int currentX, int currentY, int currentRotation);
	void drawBoard(Board board);
	void drawHeld(int heldShape);
	void drawNext(int nextNum);
	void drawTop();
	void drawShadow(Shapes shape, Board board, int currentX, int currentY, int currentRotation);
	void drawText(std::string text, int x, int y, int w, int h);
	void clear();
	void destroy();
};