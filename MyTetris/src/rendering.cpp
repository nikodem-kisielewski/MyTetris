/*
*	Class that handles the rendering of the game.
*/

#include "rendering.h"
#include "shapes.h"

// Default constructor
Rendering::Rendering()
{
	window = NULL;
	renderer = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("There was an error while initializing SDL. %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("MyTetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		if (window == NULL)
		{
			printf("There was an error while creating the window. %s\n", SDL_GetError());
		}
	}

	if (TTF_Init() < 0)
	{
		printf("There was a TTF error. %s\n", TTF_GetError());
	}

	rect.x = rect.y = rect.w = rect.h = 0;
	texture = NULL;
	textSurface = NULL;
	font = TTF_OpenFont("../Dependencies/pixel.ttf", 64);
	textColor = { 255, 255, 255, 255 };
}

// Makes a call to RenderPresent
void Rendering::drawUpdate()
{
	SDL_RenderPresent(renderer);
}

// Draw the piece
void Rendering::drawShape(Shapes shape, int currentX, int currentY, int currentRotation)
{
	// Get the information for the current shape
	std::string currentShape = shape.getShape();
	int currentNum = shape.currentShape;
	int maxIndex = shape.maxIndex();

	SDL_SetRenderDrawColor(renderer, shapeColors[currentNum].r, shapeColors[currentNum].g, shapeColors[currentNum].b, shapeColors[currentNum].a);
	for (int x = 0; x < maxIndex; x++)
		for (int y = 0; y < maxIndex; y++)
			if (currentShape[shape.rotate(x, y, currentRotation)] == 'X')
			{
				rect.x = (currentX + x) * GRID_SIZE;
				rect.y = (currentY + y) * GRID_SIZE + TOP_OFFSET;
				rect.w = rect.h = GRID_SIZE;
				SDL_RenderFillRect(renderer, &rect);
			}
}

// Draws the board
void Rendering::drawBoard(Board board)
{
	for (int x = 0; x < BOARD_WIDTH; x++)
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			// Change the render color based on the board piece
			int index = board.getItemAt(x, y);
			SDL_SetRenderDrawColor(renderer, boardColors[index].r, boardColors[index].g, boardColors[index].b, boardColors[index].a);

			// Set the parameters of the rectangle and draw it
			rect.x = x * GRID_SIZE;
			rect.y = y * GRID_SIZE + TOP_OFFSET;
			rect.w = rect.h = GRID_SIZE;
			SDL_RenderFillRect(renderer, &rect);
		}
}

// Draws the next shape
void Rendering::drawNext(int nextNum)
{
	// Get the information for the current shape
	Shapes shape;
	shape.currentShape = nextNum;
	std::string nextShape = shape.getShape();
	int maxIndex = shape.maxIndex();

	SDL_SetRenderDrawColor(renderer, shapeColors[nextNum].r, shapeColors[nextNum].g, shapeColors[nextNum].b, shapeColors[nextNum].a);
	for (int x = 0; x < maxIndex; x++)
		for (int y = 0; y < maxIndex; y++)
			if (nextShape[shape.rotate(x, y, 0)] == 'X')
			{
				rect.x = (x + BOARD_WIDTH) * GRID_SIZE - 10;
				// Offset to account for the position of the box shape
				if (nextNum == 1)
					rect.y = (y * GRID_SIZE) + TOP_OFFSET - 30;
				else rect.y = (y * GRID_SIZE) + TOP_OFFSET;
				rect.w = rect.h = GRID_SIZE;
				SDL_RenderFillRect(renderer, &rect);
			}
}

// Draws the top of the screen
void Rendering::drawTop()
{
	SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
	for (int x = 0; x < SCREEN_WIDTH / GRID_SIZE; x++)
		for (int y = 0; y < TOP_OFFSET / GRID_SIZE; y++)
		{
			if (x * GRID_SIZE > 10 || y * GRID_SIZE > 4)
			{
				rect.x = x * GRID_SIZE;
				rect.y = y * GRID_SIZE;
				rect.w = rect.h = GRID_SIZE;
				SDL_RenderFillRect(renderer, &rect);
			}
		}
}

// Draws "shadow" of piece that shows where the piece will go
void Rendering::drawShadow(Shapes shape, Board board, int currentX, int currentY, int currentRotation)
{
	int maxIndex = shape.maxIndex();
	std::string currentShape = shape.getShape();
	int possibleY = currentY;

	SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
	while (checkCollision(shape, board, currentRotation, currentX, possibleY + 1))
		possibleY++;
	for (int x = 0; x < maxIndex; x++)
		for (int y = 0; y < maxIndex; y++)
			if (currentShape[shape.rotate(x, y, currentRotation)] == 'X')
			{
				rect.x = (currentX + x) * GRID_SIZE;
				rect.y = (possibleY + y) * GRID_SIZE + TOP_OFFSET;
				rect.w = rect.h = GRID_SIZE;
				SDL_RenderDrawRect(renderer, &rect);
			}
}

// Draws text on screen given the text, X and Y position, and width and height
void Rendering::drawText(std::string text, int x, int y, int w, int h)
{
	textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;
	SDL_RenderCopy(renderer, texture, NULL, &rect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(texture);
}

// Clears the the screen to gray
void Rendering::clear()
{
	SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
	SDL_RenderClear(renderer);
}

// Destroys the window and renderer
void Rendering::destroy()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}