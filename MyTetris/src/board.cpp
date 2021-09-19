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
	playField = new unsigned char[BOARD_WIDTH * BOARD_HEIGHT];
	for (int x = 0; x < BOARD_WIDTH; x++)
		for (int y = 0; y < BOARD_HEIGHT; y++)
			// Sets the area of the board to either 0 or 8 depending on whether the area is a border or and empty space
			playField[(y * BOARD_WIDTH) + x] = (x == 0 || x == BOARD_WIDTH - 1 || y == BOARD_HEIGHT - 1) ? 8 : 0;
}

// Adds a piece to the playing field
void Board::addPiece(Shapes shape, int currentRotation, int currentX, int currentY)
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
				playField[(currentY + y) * BOARD_WIDTH + (currentX + x)] = shape.currentShape +1;
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
	playField[(y * BOARD_WIDTH) + x] = 9;

}

// Sets the board item at the specified position to the board value above it
void Board::setAbove(int x, int y)
{
	playField[y * BOARD_WIDTH + x] = playField[(y - 1) * BOARD_WIDTH + x];
}

// Sets the board item at the specified x position to an empty value (used for clearing lines)
void Board::setEmpty(int x)
{
	playField[x] = 0;
}