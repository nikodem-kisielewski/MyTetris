#pragma once

#include "shapes.h"

#define BOARD_WIDTH 12
#define BOARD_HEIGHT 21

/*
*	Class that initializes the game board, stores board states, and adds shapes to the board.
*/
class Board
{
private:
	unsigned char* playField;
public:
	Board();
	void addPiece(Shapes shape, int currentRotation, int currentX, int currentY);
	int getItemAt(int x, int y);
	void setLine(int x, int y);
	void setAbove(int x, int y);
	void setEmpty(int x);
};