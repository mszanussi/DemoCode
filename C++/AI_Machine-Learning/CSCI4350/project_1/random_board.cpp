/*
	Author: Milan Zanussi
	Class: CSCI 4350: Intro to Artificial Intelligence
	Project: Project 1: A* Search and 8-Slider Puzzle
	File: random_board.cpp
	Description: Takes an input 8-tile slider puzzle board
	as a sequence of characters and turns them into a
	shuffled board for an agent to solve.
*/
#include <iostream>
#include <cstdlib>

using namespace std;

// ShuffleBoard: Shuffles an input board randomly based on an initial
// board layout, a number of times to randomly shift pieces, and a
// seed for the random number generator
void ShuffleBoard(unsigned short board[][3], unsigned int moves, int seed, unsigned short zero_loc);

// SwapInts: Swaps two integers
void SwapPieces(unsigned short &x, unsigned short &y)
{
	x = x + y;
	y = x - y;
	x = x - y;
}

// Main function to generate random board
int main(int argc, char** argv)
{
	// Declare board variable
	unsigned short req_board[3][3];
	
	// Check command line arguments
	if (argc != 3)
	{
		cerr << "Usage: random_board <number of moves> <random number seed>" << endl;
		return -1;
	}
	
	// Read command line arguments
	unsigned int req_moves = atoi(argv[1]);
	int req_seed = atoi(argv[2]);
	unsigned short req_zero_loc;
	
	// Read board configuration from standard input
	for(int j = 0; j < 3; ++j)
	{
		for(int i = 0; i < 3; ++i)
		{
			cin >> req_board[i][j];
			
			if(req_board[i][j] == 0)
				req_zero_loc = 3*j + i;
		}
	}

	
	// Randomly shuffle the board
	ShuffleBoard(req_board, req_moves, req_seed, req_zero_loc);
	
	// Print the board
	for(int j = 0; j < 9; ++j)
	{
		if(j%3 == 2)
			cout << req_board[j%3][j/3] << "\n";
		else
			cout << req_board[j%3][j/3] << " ";
	}
	
	return 0;
}

// ShuffleBoard: Shuffles an input board randomly based on an initial
// board layout, a number of times to randomly shift pieces, and a
// seed for the random number generator
void ShuffleBoard(unsigned short board[][3], unsigned int moves, int seed, unsigned short zero_loc)
{
	// Use seed for random number generator
	srand(seed);
	
	// Parse initial position of zero
	unsigned short zrow = zero_loc%3;
	unsigned short zcol = zero_loc/3;
	
	// Shuffle board with random series of moves
	for(int k = 0; k < moves; ++k)
	{
		// Select a random move out of four possible directions
		int direction = rand()%4;
		
		switch(direction)
		{
			case(0):	// Move right if valid
			
				// If the move is valid
				if( zcol != 2 )
				{
					SwapPieces(board[zrow][zcol],board[zrow][zcol+1]);
					++zcol;
				}
				
				// If move is invalid
				else
					--k;	// Redo the current step
					
				break;
				
			case(1):	// Move up if valid
			
							
				// If the move is valid
				if( zrow != 0 )
				{
					SwapPieces(board[zrow][zcol],board[zrow-1][zcol]);
					--zrow;
				}
				
				// If move is invalid
				else
					--k;	// Redo the current step
			
				break;
				
			case(2):	// Move left if valid
			
				// If the move is valid
				if( zcol != 0 )
				{
					SwapPieces(board[zrow][zcol],board[zrow][zcol-1]);
					--zcol;
				}
				
				// If move is invalid
				else
					--k;	// Redo the current step
			
				break;
				
			case(3):	// Move down if valid
			
				// If the move is valid
				if( zrow != 2 )
				{
					SwapPieces(board[zrow][zcol],board[zrow+1][zcol]);
					++zrow;
				}
				
				// If move is invalid
				else
					--k;	// Redo the current step
			
				break;
		}

	}
	
	return;
}
