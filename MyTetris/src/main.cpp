#include "game.h"

/* String array to store shapes
std::wstring shapes[7];

// Stores the rectangles that we need to draw for the shapes
SDL_Rect rect;

// Checking for a game over
bool gameOver = false;

// User input keys
SDL_Event e;

// Use time as the seed for random number generation
time_t t;

// Use a vector to store any lines that the user makes
std::vector<int> storeLine;

// Set the parameters of the window
SDL_Window* window = NULL;
const int screenWidth = 480;
const int screenHeight = 750;

// The SDL renderer
SDL_Renderer* renderer = NULL;

// The size of a single element in the grid
const int gridSize = 30;
const int topOffset = 120;

// Set the parameters of the board
const int boardWidth = 12;
const int boardHeight = 21;
unsigned char* playField = nullptr;

// Parameters for game logic
int currentShape, nextShape;
int currentRotation = 0;
int currentX = (boardWidth / 2) - 2;
int currentY;
int speed = 20;
int level = 21 - speed;
int speedCounter = 0;
bool moveDown = false;
int score = 0;
int lineCount = 0;

// Instantly drops a piece to it's lowest possible position
void hardDrop()
{
	while (checkCollision(currentShape, currentRotation, currentX, currentY + 1, boardWidth, boardHeight, playField, shapes))
		currentY++;
	moveDown = true;
}

// Checks for lines
void lineCheck(int maxIndex)
{
	for (int y = 0; y < maxIndex; y++)
	{
		if (currentY + y < boardHeight - 1)
		{
			bool line = true;
			for (int x = 1; x < boardWidth - 1; x++)
			{
				line &= (playField[(currentY + y) * boardWidth + x]) != 0;
			}
			if (line)
			{
				for (int x = 1; x < boardWidth - 1; x++)
				{
					playField[(currentY + y) * boardWidth + x] = 9;
				}
				lineCount++;
				storeLine.push_back(currentY + y);
			}
		}
	}
}

// Draw the piece
void drawShape(int maxIndex)
{
	SDL_SetRenderDrawColor(renderer, shapeColors[currentShape].r, shapeColors[currentShape].g, shapeColors[currentShape].b, shapeColors[currentShape].a);
	for (int x = 0; x < maxIndex; x++)
		for (int y = 0; y < maxIndex; y++)
			if (shapes[currentShape][rotate(x, y, currentRotation, currentShape)] == 'X')
			{
				rect.x = (currentX + x) * gridSize;
				rect.y = (currentY + y) * gridSize + topOffset;
				rect.w = rect.h = gridSize;
				SDL_RenderFillRect(renderer, &rect);
			}
}

// Draws the board
void drawBoard()
{
	for (int x = 0; x < boardWidth; x++)
		for (int y = 0; y < boardHeight; y++)
		{
			// Change the render color based on the board piece
			int index = playField[x + (boardWidth * y)];
			SDL_SetRenderDrawColor(renderer, boardColors[index].r, boardColors[index].g, boardColors[index].b, boardColors[index].a);

			// Set the parameters of the rectangle and draw it
			rect.x = x * gridSize;
			rect.y = y * gridSize + topOffset;
			rect.w = rect.h = gridSize;
			SDL_RenderFillRect(renderer, &rect);
		}
}

// Draws the next shape
void drawNext(int maxIndex)
{
	SDL_SetRenderDrawColor(renderer, shapeColors[nextShape].r, shapeColors[nextShape].g, shapeColors[nextShape].b, shapeColors[nextShape].a);
	for (int x = 0; x < maxIndex; x++)
		for (int y = 0; y < maxIndex; y++)
			if (shapes[nextShape][rotate(x, y, 0, nextShape)] == L'X')
			{
				rect.x = (x + boardWidth) * gridSize - 10;
				if (nextShape == 1)
					rect.y = (y * gridSize) + topOffset - 30;
				else rect.y = (y * gridSize) + topOffset;
				rect.w = rect.h = gridSize;
				SDL_RenderFillRect(renderer, &rect);
			}
}

// Creates the game window and the renderer
void initializeScreen()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("There was an error while initializing SDL. %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("MyTetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
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
}

// Draws "shadow" of piece that shows where the piece will go
void drawShadow(int maxIndex)
{
	int possibleY = currentY;
	while (possibleY + 1 > currentY && checkCollision(currentShape, currentRotation, currentX, possibleY + 1, boardWidth, boardHeight, playField, shapes))
		possibleY++;
	for (int x = 0; x < maxIndex; x++)
		for (int y = 0; y < maxIndex; y++)
			if (shapes[currentShape][rotate(x, y, currentRotation, currentShape)] == 'X')
			{
				rect.x = (currentX + x) * gridSize;
				rect.y = (possibleY + y) * gridSize + topOffset;
				rect.w = rect.h = gridSize;
				SDL_RenderDrawRect(renderer, &rect);
			}
}

// Draws the top of the screen
void drawTop()
{
	// Draws the top of the screen
	SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
	for (int x = 0; x < screenWidth / gridSize; x++)
		for (int y = 0; y < topOffset / gridSize; y++)
		{
			rect.x = x * gridSize;
			rect.y = y * gridSize;
			rect.w = rect.h = gridSize;
			SDL_RenderFillRect(renderer, &rect);
		}
}

// Animation for getting a new line (lines flash white)
void removeLines(Board board, std::vector)
{
	if (!storeLine.empty())
	{
		SDL_RenderPresent(renderer);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		for (auto& v : storeLine)
		{
			for (int x = 1; x < BOARD_WIDTH - 1; x++)
			{
				for (int y = v; y > 0; y--)
				{
					board.setAbove(x, y);
				}
				playField[x] = 0;
			}
			storeLine.clear();
		}
	}
}

int main(int argc, char* argv[])
{
	/*
	createShapes(shapes);

	initializeScreen();

	// Music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music* theme = Mix_LoadMUS("TetrisTheme.wav");
	Mix_PlayMusic(theme, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

	// Text rendering
	TTF_Font* font = TTF_OpenFont("Minecraft.ttf", 64);
	SDL_Color fColor = { 255, 255, 255, 255 };
	SDL_Surface* textSurface;
	SDL_Texture* text;
	SDL_Rect textRect;
	std::string scoreText, levelText;

	// Create the playing field
	playField = new unsigned char[boardWidth * boardHeight];
	initializeBoard(boardWidth, boardHeight, playField);

	// Get a random number to set the first piece
	srand((unsigned) time(&t));
	currentShape = rand() % 7;
	nextShape = rand() % 7;

	// Determine the maximum x and y indices depending on the shape
	int maxIndex, nextMax;
	if (currentShape == 0 || currentShape == 1)
		maxIndex = 4, currentY = -1;
	else maxIndex = 3;

	if (nextShape == 0 || nextShape == 1)
		nextMax = 4;
	else nextMax = 3;

	// The main game loop
	while (!gameOver)
	{
		// Timing of the game
		// One game tick is 50ms
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		speedCounter++;
		moveDown = (speedCounter == speed);

		// Clear the screen of previous renders that are not needed anymore
		SDL_RenderClear(renderer);

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
					if (checkCollision(currentShape, currentRotation + 1, currentX, currentY, boardWidth, boardHeight, playField, shapes))
					{
						currentRotation++;
					}
					break;

				case SDLK_DOWN:
					if (checkCollision(currentShape, currentRotation, currentX, currentY + 1, boardWidth, boardHeight, playField, shapes))
					{
						currentY++;
					}
					break;

				case SDLK_RIGHT:
					if (checkCollision(currentShape, currentRotation, currentX + 1, currentY, boardWidth, boardHeight, playField, shapes))
					{
						currentX++;
					}
					break;

				case SDLK_LEFT:
					if (checkCollision(currentShape, currentRotation, currentX - 1, currentY, boardWidth, boardHeight, playField, shapes))
					{
						currentX--;
					}
					break;
				case SDLK_SPACE:
					hardDrop();
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

			// Check if the piece can be moved down
			if (checkCollision(currentShape, currentRotation, currentX, currentY + 1, boardWidth, boardHeight, playField, shapes))
			{
				currentY++;
			}
			// If it can't, add it to the play field
			else
			{
				// Place the current piece into the field
				for (int x = 0; x < maxIndex; x++)
				{
					for (int y = 0; y < maxIndex; y++)
					{
						if (shapes[currentShape][rotate(x, y, currentRotation, currentShape)] != L'.')
						{
							playField[(currentY + y) * boardWidth + (currentX + x)] = currentShape + 1;
						}
					}
				}

				lineCheck(maxIndex);

				// Increase the player's score for each piece and line
				score += 50;
				if (!storeLine.empty()) score += (1 << storeLine.size()) * 100;

				// Speed the game up every 50 pieces
				if (lineCount % 10 == 0 && speed > 1)
				{
					speed--;
					level = 21 - speed;
				}

				// Get the next piece
				currentX = (boardWidth / 2) - 2;
				currentRotation = 0;
				currentShape = nextShape;
				maxIndex = nextMax;
				nextShape = rand() % 7;

				// Determine new Y index based on the new shape
				if (maxIndex == 4)
					currentY = -1;
				else currentY = 0;

				// Determine the x and y indices depending on the new next shape
				if (nextShape == 0 || nextShape == 1)
					nextMax = 4;
				else nextMax = 3;

				// If the new piece doesn't fit, game over
				gameOver = !checkCollision(currentShape, currentRotation, currentX, currentY, boardWidth, boardHeight, playField, shapes);
			}
		}

		drawBoard();

		drawShape(maxIndex);

		drawShadow(maxIndex);

		drawNext(nextMax);

		drawTop();

		// Text
		scoreText = "Score: " + std::to_string(score);
		levelText = "Level: " + std::to_string(level);

		// Score text rendering
		textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), fColor);
		text = SDL_CreateTextureFromSurface(renderer, textSurface);
		textRect.x = 10;
		textRect.y = 10;
		textRect.h = 40;
		textRect.w = 250;
		SDL_RenderCopy(renderer, text, NULL, &textRect);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(text);

		// Level text rendering
		textSurface = TTF_RenderText_Solid(font, levelText.c_str(), fColor);
		text = SDL_CreateTextureFromSurface(renderer, textSurface);
		textRect.x = 10;
		textRect.y = 60;
		textRect.h = 40;
		textRect.w = 150;
		SDL_RenderCopy(renderer, text, NULL, &textRect);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(text);
		SDL_RenderPresent(renderer);

		// Next shape text rendering
		textSurface = TTF_RenderText_Solid(font, "Next Shape", fColor);
		text = SDL_CreateTextureFromSurface(renderer, textSurface);
		textRect.x = boardWidth * gridSize - 15;
		textRect.y = (gridSize * 2) + topOffset + 10;
		textRect.h = 30;
		textRect.w = 120;
		SDL_RenderCopy(renderer, text, NULL, &textRect);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(text);
		SDL_RenderPresent(renderer);

		removeLines();

		SDL_RenderPresent(renderer);
	}

	// Destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	// Free memory
	Mix_FreeMusic(theme);
	delete[] playField;

	// Quit SDL
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();

	return 0;
}
*/

int main(int argc, char* argv[])
{
	gameLoop();
	return 0;
}