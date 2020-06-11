#include <cstdlib>
#include <iostream>
#include <math.h>
#include "moveFinder.h"
using namespace std;

int indexFromString(const string move, const int boardSize)
{
	int column = move[0] - 97;
	int row = atoi(&move[1]) - 1;
	int r = row * boardSize + column;
	if (r < 0)
	{
		cout << "Error reading string" << endl;
	}
	return r;
}

void printBoard(int** const board, const int boardSize)
{
	cout << "  ";
	for (int i = 0; i < boardSize; i++)
	{
		int asciiVal = i + 97;
		char asciiChar = asciiVal;
		cout << asciiChar << " ";
	}
	cout << endl;
	for (int i = 0; i < boardSize; i++)
	{
		cout << i + 1 << " ";
		for (int j = 0; j < boardSize; j++)
		{
			if (board[i][j] == -1)
			{
				cout << "X ";
			}
			else if (board[i][j] == 1)
			{
				cout << "O ";
			}
			else
			{
				cout << "_ ";
			}
		}
		cout << endl;
	}
}

int main(int argc, char* argv[])
{
	int boardSize = 11;
	int myColor = 1;
	int depth = 4;
	
	// Read Command Line Arguments

	for (int i = 0; i < argc; i++)
	{
		if ((string)argv[i] == "-l")
		{
			myColor = -1;
		}
		else if ((string)argv[i] == "-n")
		{
			int n = atoi(argv[i + 1]);
			if ((n == 1 || n == 2) && i + 2 < argc)
			{
				n = n * 10 + atoi(argv[i + 2]);
			}
			boardSize = n;
		}
	}
	if (boardSize == 5)
	{
		depth = -1;
	}
	cout << "Depth: " << depth << endl;

	// Initialise game board
	int** board = new int* [boardSize];
	for (int i = 0; i < boardSize; i++)
	{
		board[i] = new int[boardSize];
		for (int j = 0; j < boardSize; j++)
		{
			board[i][j] = 0;
		}
	}

	// Start Game
	printBoard(board, boardSize);
	int move = 0;
	while (move < boardSize * boardSize)
	{
		int placedIndex;
		int row;
		int column;

		// Skip opponent if myColor is dark and first move
		if (myColor == 1 || move != 0)
		{
			// Get and place opponent piece
			string move = "";
			cin >> move;
			placedIndex = indexFromString(move, boardSize);
			row = placedIndex / boardSize;
			column = placedIndex % boardSize;
			if (board[row][column] != 0)
			{
				cout << "Opponent chosen spot already taken!" << endl;
				break;
			}
			else
			{
				int asciiVal = column + 97;
				char asciiChar = asciiVal;
				cout << "Move Played: " << asciiChar << row + 1 << endl;
			}
			board[row][column] = myColor * -1;
			
			printBoard(board, boardSize);
			int r = checkForWin(board, boardSize);
			if (r == -1)
			{
				cout << "Dark Wins" << endl;
				break;
			}
			else if (r == 1)
			{
				cout << "Light Wins" << endl;
				break;
			}
			else if (r == 2)
			{
				cout << "Tie" << endl;
				break;
			}
		}

		//Calculate and place my piece
		if (myColor == -1 && move == 0)
		{
			placedIndex = (boardSize / 2) * boardSize + (boardSize / 2);
		}
		else
		{
			placedIndex = calcNextMove(board, boardSize, myColor, depth, boardSize * boardSize - move);
		}
		row = placedIndex / boardSize;
		column = placedIndex % boardSize;
		if (board[row][column] != 0)
		{
			cout << row << "," << column << endl;
			cout << "My chosen spot already taken!" << endl;
			break;
		}
		else
		{
			int asciiVal = column + 97;
			char asciiChar = asciiVal;
			cout << "Move Played: " << asciiChar << row + 1 << endl;
		}
		board[row][column] = myColor;
		move += 1;
		
		printBoard(board, boardSize);
		int r = checkForWin(board, boardSize);
		if (r == -1)
		{
			cout << "Dark Wins" << endl;
			break;
		}
		else if (r == 1)
		{
			cout << "Light Wins" << endl;
			break;
		}
		else if (r == 2)
		{
			cout << "Tie" << endl;
			break;
		}
		
	}
	cout << "Game Over" << endl;

	// Clear Memory
	for (int i = 0; i < boardSize; i++)
	{
		delete[] board[i];
	}
	delete[] board;

	return 0;
}