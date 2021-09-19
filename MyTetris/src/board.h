/*
*	Class that initializes the game board, stores board states, and adds shapes to the board.
*
*	@author Nikodem Kisielewski
*
*/

#pragma once

#include "shapes.h"

#define BOARD_WIDTH 12
#define BOARD_HEIGHT 21

class Board
{
private:
	unsigned char playField[BOARD_WIDTH * BOARD_HEIGHT];
public:
	Board();
	void addPiece(Shapes shape, Board board, int currentRotation, int currentX, int currentY);
	int getItemAt(int x, int y);
	void setLine(int x, int y);
	void setAbove(int x, int y);
	void setEmpty(int x);
};