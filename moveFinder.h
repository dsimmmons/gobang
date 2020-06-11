#pragma once
#include <iostream>
using namespace std;

int getBoardScore(int** const board, const int boardSize, int beta, int alpha, const int myColor, int const origColor, const int currentDepth, const int maxDepth, const int spacesLeft);

int calcNextMove(int** const board, const int boardSize, const int myColor, const int maxDepth, int const spacesLeft);

int boardScore(int** const board, const int boardSize, int const origColor);

int chain(int** const board, const int boardSize, int const color, int const row, int const col, const int vD, const int hD, int c);

int checkForWin(int** const board, const int boardSize);