/*
*	Class that initializes the game board, stores board states, and adds shapes to the board.
*
*	@author Nikodem Kisielewski
*
*/

#include "board.h"

// Default constructor
Board::Board()
{
	for (int x = 0; x < BOARD_WIDTH; x++)
		for (int y = 0; y < BOARD_HEIGHT; y++)
			if (x == 0 || x == BOARD_WIDTH - 1 || y == BOARD_HEIGHT - 1) {
				// Sets the boundaries of the board to 8
				playField[(y * BOARD_WIDTH) + x] = 8;
			}
			else
				// Sets empty positions to 0
				playField[(y * BOARD_HEIGHT) + x] = 0;
}

// Adds a piece to the playing field
void Board::addPiece(Shapes shape, Board board, int currentRotation, int currentX, int currentY)
{
	// Get the parameters of the current shape
	std::string thisShape = shape.getShape();
	int max = shape.maxIndex();

	for (int x = 0; x < max; x++)
		for (int y = 0; y < max; y++)
			if (thisShape[shape.rotate(x, y, currentRotation)] == 'X')
			{
				// Add the number representation of the current shape (+1 since 0 is an empty space)
				// to the playing field based on the current x position, current y position,
				// and the shape's current rotation
				board.playField[(currentY + y) * BOARD_WIDTH + (currentX + x)] = shape.currentShape; +1;
			}
}

// Returns the item in the gameboard at the specified x and y values
int Board::getItemAt(int x, int y)
{
	return playField[x + (BOARD_WIDTH * y)];
}

// Sets the board item at the specified position to a line value
void Board::setLine(int x, int y)
{
	playField[y * BOARD_WIDTH + x] = 9;

}

// Sets the board item at the specified position to the board value above it
void Board::setAbove(int x, int y)
{
	playField[y * BOARD_WIDTH + x] = playField[(y - 1) * BOARD_WIDTH + x];
}

// Sets the board item at the specified x position to an empty value
void Board::setEmpty(int x)
{
	playField[x] = 0;

}