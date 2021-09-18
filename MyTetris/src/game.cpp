// The core game mechanics of tetris and user input

#include "game.h"

// Check to see if the piece will fit in the next calculated position
bool checkCollision(Shapes shape, Board board, int currentRotation, int xPos, int yPos)
{
	// Max index of the current piece
	int maxIndex = shape.maxIndex();
	std::string currentShape = shape.getShape();

	for (int x = 0; x < maxIndex; x++)
		for (int y = 0; y < maxIndex; y++)
		{
			int shapeIndex = shape.rotate(x, y, currentRotation);
			int fieldItem = board.getItemAt(yPos + y, xPos + x);
			if (xPos + x >= 0 && xPos + x < BOARD_WIDTH)
			{
				if (yPos + y >= 0 && yPos + y < BOARD_HEIGHT)
				{
					if (currentShape[shapeIndex] == 'X' && fieldItem != 0)
					{
						return false;
					}
				}
			}
		}
	return true;
}

// Instantly drops a piece to it's lowest possible position
bool hardDrop(Shapes shape, Board board, int currentRotation, int currentX, int& currentY)
{
	while (checkCollision(shape, board, currentRotation, currentX, currentY + 1))
		currentY++;
	return true;
}

void lineCheck(Board board, std::vector<int> lines, int& lineCount, int maxIndex, int currentY)
{
	for (int y = 0; y < maxIndex; y++)
	{
		if (currentY + y < BOARD_HEIGHT - 1)
		{
			bool line = true;
			for (int x = 1; x < BOARD_WIDTH - 1; x++)
			{
				int fieldItem = board.getItemAt(x, y);
				line &= (fieldItem) != 0;
			}
			if (line)
			{
				for (int x = 1; x < BOARD_WIDTH - 1; x++)
				{
					board.setLine(x, y);
				}
				lineCount++;
				lines.push_back(currentY + y);
			}
		}
	}
}

// Animation for getting a new line (lines flash white)
void removeLines(Board board, std::vector<int> storeLine)
{
	if (!storeLine.empty())
	{
		for (auto& v : storeLine)
		{
			for (int x = 1; x < BOARD_WIDTH - 1; x++)
			{
				for (int y = v; y > 0; y--)
				{
					board.setAbove(x, y);
				}
				board.setEmpty(x);
			}
			storeLine.clear();
		}
	}
}

void gameLoop()
{
	Shapes shape;
	Board board;
	Rendering render;

	// Booleans for game logic
	bool gameOver = false;
	bool moveDown = false;

	// Integers used for game logic
	int currentX = (BOARD_WIDTH / 2) - 2;
	int currentY = 0;
	int currentRotation = 0;
	int nextShape;
	int speed = 20;
	int speedCounter = 0;
	int level = 21 - speed;
	int score = 0;
	int lineCount = 0;

	// Text
	std::string scoreText = "Score: " + score;
	std::string levelText = "Level: " + level;

	// Vector for handling lines
	std::vector<int> storeLine;

	// Use time as the seed for random number generation
	time_t t;

	// Tracking player input
	SDL_Event e;

	// Set up random number generation
	srand((unsigned)time(&t));
	shape.currentShape = rand() % 7;
	nextShape = rand() % 7;

	while (!gameOver)
	{
		// Timing of the game
		// One game tick is 50ms
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		speedCounter++;
		moveDown = (speedCounter == speed);

		// Clear the screen of previous renders that are not needed anymore
		render.clear();

		// User input
		while (SDL_PollEvent(&e) != 0)
		{
			// User clicks the 'X' button
			if (e.type == SDL_QUIT)
			{
				gameOver = true;
			}
			// User is pressing down a key
			if (e.type == SDL_KEYDOWN)
			{
				// Gets the keycode for the key being pressed
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					if (checkCollision(shape, board, currentRotation + 1, currentX, currentY))
					{
						currentRotation++;
					}
					break;

				case SDLK_DOWN:
					if (checkCollision(shape, board, currentRotation, currentX, currentY + 1))
					{
						currentY++;
					}
					break;

				case SDLK_RIGHT:
					if (checkCollision(shape, board, currentRotation, currentX + 1, currentY))
					{
						currentX++;
					}
					break;

				case SDLK_LEFT:
					if (checkCollision(shape, board, currentRotation, currentX - 1, currentY))
					{
						currentX--;
					}
					break;
				case SDLK_SPACE:
					hardDrop(shape, board, currentRotation, currentX, currentY);
					break;

				case SDLK_ESCAPE:
					gameOver = true;
					break;
				}
			}
		}

		// Force the piece down
		if (moveDown)
		{
			// Reset the speed counter
			speedCounter = 0;

			// Check to see if the piece can be moved down
			if (checkCollision(shape, board, currentRotation, currentX, currentY + 1))
			{
				currentY++;
			}
			// If it can't, add it to the playing field
			else
			{
				board.addPiece(shape, board, currentRotation, currentX, currentY);

				// Check for lines
				lineCheck(board, storeLine, lineCount, shape.maxIndex(), currentY);

				// Increase the player's score for each piece and line
				score += 50;
				if (!storeLine.empty()) score += (1 << storeLine.size()) * 100;

				// Speed the game up every 10 lines
				if (lineCount % 10 == 0 && speed > 1)
				{
					speed--;
					level = 21 - speed;
				}

				// Get the next piece
				currentX = (BOARD_WIDTH / 2) - 2;
				currentRotation = 0;
				shape.currentShape = nextShape;
				currentY = (shape.maxIndex() == 4) ? -1 : 0;
				nextShape = rand() % 7;

				// If the new piece does not fit, game over
				gameOver = !checkCollision(shape, board, currentRotation, currentX, currentY);
			}
		}

		//Rendering the game screen
		render.drawBoard(board);
		render.drawShadow(shape, board, currentX, currentY, currentRotation);
		render.drawShape(shape, currentX, currentY, currentRotation);
		render.drawNext(nextShape);
		render.drawTop();
		render.drawText(scoreText, 10, 10, 150, 40);
		render.drawText(levelText, 10, 60, 120, 40);
		render.drawText("Next Shape", BOARD_WIDTH * GRID_SIZE - 15, 60 + TOP_OFFSET + 10, 120, 30);

		// Line clearing animation
		render.drawUpdate();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		removeLines(board, storeLine);

		render.drawUpdate();
	}
	render.destroy();

	// Quit SDL
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();
}