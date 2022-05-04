#include "game.h"

/*
*	Tetris core game mechanics, handling user input, playing music, and the entire gameloop.
*/

// Tracking the gamestate
GameState currState;

// Checks whether the user wishes to mute the game
bool muted;

// Final score after a game over
int finalScore;

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
			int fieldItem = board.getItemAt(xPos + x, yPos + y);
			if (xPos + x >= 0 && xPos + x < BOARD_WIDTH)
			{
				if (yPos + y >= 0 && yPos + y < BOARD_HEIGHT)
				{
					if (currentShape[shapeIndex] == 'X' && fieldItem != 0)
						return false;
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

// Checks for lines and adds them to the line vector
void lineCheck(Board board, std::vector<int> &lines, int &lineCount, int& speed, int& level, int maxIndex, int currentY)
{
	for (int y = 0; y < maxIndex; y++)
	{
		if (currentY + y < BOARD_HEIGHT - 1)
		{
			bool line = true;
			for (int x = 1; x < BOARD_WIDTH - 1; x++)
			{
				int fieldItem = board.getItemAt(x, y + currentY);
				line &= (fieldItem) != 0;
			}
			if (line)
			{
				for (int x = 1; x < BOARD_WIDTH - 1; x++)
				{
					board.setLine(x, y + currentY);
				}
				lineCount++;
				// Speed the game up every 10 lines
				if (lineCount > 0 && lineCount % 10 == 0 && speed > 1)
				{
					speed--;
					level = 21 - speed;
				}
				lines.push_back(currentY + y);
			}
		}
	}
}

// Removes any lines the player got from the board
void removeLines(Board board, std::vector<int>& storeLine)
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

// The actual game loop
void gameLoop(Rendering render)
{
	Shapes shape;
	Board board;

	// Booleans for game logic
	bool gameOver = false;
	bool moveDown = false;
	bool alreadyHeld = false;

	// Integers used for game logic
	int currentX = (BOARD_WIDTH / 2) - 2;
	int currentY = 0;
	int currentRotation = 0;
	int nextShape;
	int heldPiece = 8;
	int speed = 20;
	int speedCounter = 0;
	int level = 21 - speed;
	int score = 0;
	int lineCount = 0;

	// Text
	std::string scoreText = "Score: " + std::to_string(score);
	std::string levelText = "Level: " + std::to_string(level);

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

	// Music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music* theme = Mix_LoadMUS("../Dependencies/TetrisTheme.wav");
	Mix_PlayMusic(theme, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

	while (!gameOver)
	{
		// Timing of the game
		// One game tick is 50ms
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		speedCounter++;
		moveDown = (speedCounter == speed);

		// User input
		while (SDL_PollEvent(&e) != 0)
		{
			// User clicks the 'X' button
			if (e.type == SDL_QUIT)
			{
				gameOver = true;
				currState = GameState::QUIT;
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
					moveDown = hardDrop(shape, board, currentRotation, currentX, currentY);
					break;

				case SDLK_m:
					if (muted) {
						Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
						muted = false;
					}
					else {
						Mix_VolumeMusic(0);
						muted = true;
					}
					break;
				case SDLK_c:
					if (!alreadyHeld)
					{
						if (heldPiece == 8)
						{
							heldPiece = shape.currentShape;
							shape.currentShape = nextShape;
							nextShape = rand() % 7;
						}
						else
						{
							int tempPiece = shape.currentShape;
							shape.currentShape = heldPiece;
							heldPiece = tempPiece;
						}
						currentX = (BOARD_WIDTH / 2) - 2;
						currentY = 0;
						currentRotation = 0;
						alreadyHeld = true;
					}
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
 				board.addPiece(shape, currentRotation, currentX, currentY);

				// Check for lines
				lineCheck(board, storeLine, lineCount, speed, level, shape.maxIndex(), currentY);

				// Increase the player's score for each piece and line
				score += 50;
				if (!storeLine.empty()) score += (1 << storeLine.size()) * 100;

				// Remove any lines that the user got
				if (!storeLine.empty())
				{
					// Before we remove the lines, draw the board to simulate a line clearing animation
					render.drawBoard(board);
					render.drawUpdate();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					removeLines(board, storeLine);
				}

				// Get the next piece
				currentX = (BOARD_WIDTH / 2) - 2;
				currentRotation = 0;
				shape.currentShape = nextShape;
				currentY = (shape.maxIndex() == 4) ? -1 : 0;
				nextShape = rand() % 7;

				// If the new piece does not fit, game over
				gameOver = !checkCollision(shape, board, currentRotation, currentX, currentY);

				alreadyHeld = false;
			}
		}

		//Rendering the game screen
		render.clear();
		render.drawBoard(board);
		render.drawShadow(shape, board, currentX, currentY, currentRotation);
		render.drawShape(shape, currentX, currentY, currentRotation);
		render.drawNext(nextShape);
		if (heldPiece < 7) render.drawHeld(heldPiece);
		render.drawTop();

		// Text rendering
 		scoreText = "Score: " + std::to_string(score);
		levelText = "Level: " + std::to_string(level);
		render.drawText(scoreText, 10, 10, std::strlen(scoreText.c_str()) * GRID_SIZE, 40);
		render.drawText(levelText, 10, 60, std::strlen(levelText.c_str()) * GRID_SIZE, 40);
		render.drawText("Next Shape", BOARD_WIDTH * GRID_SIZE - 15, 60 + TOP_OFFSET + 10, 120, 30);
		render.drawText("Held Shape", BOARD_WIDTH* GRID_SIZE - 15, 60 + TOP_OFFSET * 2.5 + 10, 120, 30);

		render.drawUpdate();
	}
	finalScore = score;
	if (currState != GameState::QUIT) currState = GameState::GAME_OVER;
}

// The main menu of the game
void menu(Rendering render)
{
	render.clear();
	render.drawText("MyTetris", SCREEN_WIDTH / 8, 30, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 10);
	render.drawText("Controls:", SCREEN_WIDTH / 8, 200, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 20);
	render.drawText("Arrow keys to move/rotate pieces", SCREEN_WIDTH / 6, 250, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 25);
	render.drawText("Space to 'hard drop' a piece", SCREEN_WIDTH / 6, 300, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 25);
	render.drawText("C to hold a piece", SCREEN_WIDTH / 6, 350, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 25);
	render.drawText("M to mute/unmute music", SCREEN_WIDTH / 6, 400, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 25);
	render.drawText("Escape to quit the game", SCREEN_WIDTH / 6, 450, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 25);
	render.drawText("Press enter to start", SCREEN_WIDTH / 8, 550, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 15);
	render.drawUpdate();
	while (currState == GameState::START_MENU)
	{
		// User input
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			// User clicks the 'X' button
			if (e.type == SDL_QUIT)
			{
				currState = GameState::QUIT;
				break;
			}
			// User is pressing down a key
			if (e.type == SDL_KEYDOWN)
			{
				// Gets the keycode for the key being pressed
				switch (e.key.keysym.sym)
				{
				case SDLK_RETURN:
					currState = GameState::IN_GAME;
					break;
				case SDLK_m:
					muted = true;
					break;
				case SDLK_ESCAPE:
					currState = GameState::QUIT;
					break;
				}
			}
		}
	}
}

// Game over screen, shows the user's score 
void gameOver(Rendering render)
{
	Mix_VolumeMusic(0);
	while (currState == GameState::GAME_OVER)
	{
		render.clear();
		render.drawText("GAME OVER", SCREEN_WIDTH / 8, 30, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 10);
		std::string scoreText = "Final Score: " + std::to_string(finalScore);
		render.drawText(scoreText, SCREEN_WIDTH / 4, 200, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 20);
		render.drawText("Press enter to restart", SCREEN_WIDTH / 8, 300, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 15);
		render.drawText("Press escape to exit", SCREEN_WIDTH / 8, 400, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 15);
		render.drawUpdate();
		// User input
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			// User clicks the 'X' button
			if (e.type == SDL_QUIT)
			{
				currState = GameState::QUIT;
				break;
			}
			// User is pressing down a key
			if (e.type == SDL_KEYDOWN)
			{
				// Gets the keycode for the key being pressed
				switch (e.key.keysym.sym)
				{
				case SDLK_RETURN:
					currState = GameState::IN_GAME;
					break;
				case SDLK_m:
					muted = true;
					break;
				case SDLK_ESCAPE:
					currState = GameState::QUIT;
					break;
				}
			}
		}
	}
}

// Controls the entire program and decides which menu to show to the user
void stateLoop()
{
	Shapes shape;
	Board board;
	Rendering render;

	while (currState != GameState::QUIT)
	{
		switch (currState)
		{
		case GameState::START_MENU:
			menu(render);
			break;
		case GameState::IN_GAME:
			gameLoop(render);
			break;
		case GameState::GAME_OVER:
			gameOver(render);
			break;
		}
	}
	render.destroy();

	// Quit SDL
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();
}