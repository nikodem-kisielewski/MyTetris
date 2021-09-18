#pragma once

#include "rendering.h"
#include <iostream>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <vector>

bool checkCollision(Shapes shape, Board board, int currentRotation, int xPos, int yPos);
bool hardDrop(Shapes shape, Board board, int currentRotation, int currentX, int& currentY);
void lineCheck(Board board, std::vector<int> lines, int& lineCount, int maxIndex, int currentY);
void removeLines(Board board, std::vector<int> storeLine);
void gameLoop();