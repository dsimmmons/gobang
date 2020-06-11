#include "moveFinder.h"

int chain(int** const board, const int boardSize, int const color, int const row, int const col, const int vD, const int hD, int c) // Direction Score
{
	if ((row >= 0) && (row < boardSize) && (col >= 0) && (col < boardSize)) // Spot is in bounds
	{
		if (board[row][col] == color * -1) // Found opponent color
		{
			return -1;
			/*
			if (c == 4)
			{
				return -1;
			}
			else
			{
				return chain(board, boardSize, color, row + vD, col + hD, vD, hD, c + 1) * -1;
			}
			*/
		}
		else if (board[row][col] == 0) // Found empty space
		{
			if (c == 4)
			{
				return 1; // Try 2 instead
			}
			else
			{
				return chain(board, boardSize, color, row + vD, col + hD, vD, hD, c + 1);
			}
		}
		else // Found same color
		{
			if (c == 4)
			{
				return 5; // Try 2 instead
			}
			else
			{
				return chain(board, boardSize, color, row + vD, col + hD, vD, hD, c + 1) * 5;
			}
		}
	}
	else // Spot is out of bounds
	{
		return 0;
	}
}

int boardScore(int** const board, const int boardSize, int const origColor)
{
	int score = 0;
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			if (board[i][j] != 0)
			{
				// Iterate through each direction
				for (int vD = -1; vD < 2; vD++)
				{
					for (int hD = -1; hD < 2; hD++)
					{
						if (vD != 0 || hD != 0)
						{
							int pieceScore = chain(board, boardSize, board[i][j], i + hD, j + vD, hD, vD, 1);
							score += board[i][j] * pieceScore;
						}
					}
				}
			}
		}
	}
	return score;
}

int getBoardScore(int** const board, const int boardSize, int beta, int alpha, const int myColor, int const origColor, const int currentDepth, const int maxDepth, const int spacesLeft)
{
	if (currentDepth < maxDepth && spacesLeft > 0)
	{
		int bestScore = -1 * myColor * boardSize * 100000;

		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				if (board[i][j] == 0)
				{
					board[i][j] = myColor;
					int score = getBoardScore(board, boardSize, beta, alpha, myColor * -1, origColor, currentDepth + 1, maxDepth, spacesLeft - 1);
					board[i][j] = 0;
					if (myColor == 1)
					{
						if (score > bestScore)
						{
							bestScore = score;
						}
						if (score > alpha)
						{
							alpha = score;
							if (beta <= alpha)
							{
								// Break out of both loops
								i = boardSize;
								j = boardSize;
							}
						}
					}
					else
					{
						if (score < bestScore)
						{
							bestScore = score;
						}
						if (score < beta)
						{
							beta = score;
							if (beta <= alpha)
							{
								// Break out of both loops
								i = boardSize;
								j = boardSize;
							}
						}
					}
				}
			}
		}
		return bestScore;
	}
	else
	{
		int score = boardScore(board, boardSize, origColor);
		return score;
	}
}

int calcNextMove(int** const board, const int boardSize, const int myColor, const int maxDepth, int const spacesLeft)
{
	int bestMove = -1;
	int bestScore = -1 * myColor * boardSize * 100000;
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			if (board[i][j] == 0)
			{
				board[i][j] = myColor;
				int score = 0;
				if (maxDepth != -1)
				{
					int beta = boardSize * 1000000;
					int alpha = -1 * boardSize * 1000000;
					score = getBoardScore(board, boardSize, beta, alpha, myColor * -1, myColor, 1, maxDepth, spacesLeft - 1);
				}
				else
				{
					score = boardScore(board, boardSize, myColor);
				}
				board[i][j] = 0;
				if (myColor == 1)
				{
					if (score  > bestScore || bestMove == -1)
					{
						bestScore = score;
						bestMove = i * boardSize + j;
					}
				}
				else
				{
					if (score  < bestScore || bestMove == -1)
					{
						bestScore = score;
						bestMove = i * boardSize + j;
					}
				}
			}
		}
	}
	return bestMove;
}

int checkForWin(int** const board, const int boardSize)
{
	int emptySpaces = 0;
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			if (board[i][j] != 0)
			{
				int color = board[i][j];
				for (int vD = -1; vD < 2; vD++)
				{
					for (int hD = -1; hD < 2; hD++)
					{
						if (vD != 0 || hD != 0)
						{
							int c = 1;
							int x = j;
							int y = i;
							while (x >= 0 && x < boardSize && y >= 0 && y < boardSize)
							{
								if (board[y][x] != color)
								{
									break;
								}
								if (c == 5)
								{
									return board[y][x];
								}
								x += hD;
								y += vD;
								c += 1;
							}
						}
					}
				}
			}
			else
			{
				emptySpaces += 1;
			}
		}
	}
	if (emptySpaces == 0)
	{
		return 2;
	}
	else
	{
		return 0;
	}
}