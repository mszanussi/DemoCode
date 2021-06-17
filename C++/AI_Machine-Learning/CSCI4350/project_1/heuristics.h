/*
	Author: Milan Zanussi
	Class: CSCI 4350: Intro to Artificial Intelligence
	Project: Project 1: A* Search and 8-Slider Puzzle
	File: heuristics.cpp
	Description: Header file which contains a set of heuristics
	which will be used by the A* search algorithm to try to solve
	different 8-slider puzzles.
*/
#include <cmath>

// UniformHeuristic: A* heuristic which reduces to Uniform Cost Search
int UniformHeuristic(char curr_board[][3])
{
    return 0;
}

// DisplaceHeuristic: A* heuristic which counts the number of displaced tiles
int DisplaceHeuristic(char curr_board[][3])
{
    int total = 0;

    // Iterate over all locations
    for(int k = 0; k < 9; ++k)
    {
        // If the char in the current location is not the one which belongs
        // in that location, then add one to the total
        if(curr_board[k%3][k/3] != (k + '0'))
            ++total;
    }

    return total;
}

// TaxicabHeuristic: A* heuristic which sums all of the Taxicab distances of
// each tiles current location of the tile's respective goal state.
int TaxicabHeuristic(char curr_board[][3])
{
    int total = 0;

    // Iterate over all locations
    for(int y = 0; y < 3; ++y)
    {
        for(int x = 0; x < 3; ++x)
        {
            // Ignore zero for metric
            if(curr_board[x][y] == '0')
                continue;

            // Find goal location of tile in current location
            // Using the int value of the stored char
            int goal_x = (curr_board[x][y]-'0')%3;
            int goal_y = (curr_board[x][y]-'0')/3;

            // Take taxicab metric of char from its goal location and add to total
            total += int(std::abs(goal_x - x)) + int(std::abs(goal_y - y));
        }
    }

    return total;
}

// ZeroNeighbor: Determines if '0' is a neighbor of the tile at (row,col) on a
// board configuration.
bool ZeroNeighbor(char curr_board[][3], int row, int col)
{
    // Are the row and column numbers valid?
    if( row >= 3 || row < 0 || col >= 3 || col < 0 )
        return false;

    // Is 0 to the left?
    if(row - 1 >= 0 && curr_board[row-1][col] == '0')
        return true;

    // Is 0 to the right?
    if(row + 1 <= 2 && curr_board[row+1][col] == '0')
        return true;

    // Is 0 below?
    if(col - 1 >= 0 && curr_board[row][col-1] == '0')
        return true;

    // Is 0 above?
    if(col + 1 <= 2 && curr_board[row][col+1] == '0')
        return true;

    // Then no.
    return false;

}

// InclusionHeuristic: A* heuristic which approximates distance from goal state by
// splitting the 3x3 grid into four 2x2 grids which each constitute the four corners
// of the grid, and considering which tiles should be included in each of the four
// corner grids, adding one for each set in which they exist but don't belong, adding
// one for each set in which they belong but do not exist, and subtracting one for
// each displaced tile adjacent to 0.
int InclusionHeuristic(char curr_board[][3])
{
    int total = 0;

    // Iterate over all tiles
    for(int y = 0; y < 3; ++y)
    {
        for(int x = 0; x < 3; ++x)
        {
            int local_total = 0;

            // Count incorrect truth values for inclusion of tile
            // in particular sections based on which tile is in
            // the current location
            switch(curr_board[x][y])
            {
                case('0'):
                    if(x > 1 || y > 1) ++local_total;    // Not in upper-left
                    if(x >= 1 && y <= 1) ++local_total;  // In upper-right
                    if(x <= 1 && y >= 1) ++local_total;  // In lower-right
                    if(x >= 1 && y >= 1) ++local_total;  // In lower-left
                    break;

                case('1'):
                    if(x > 1 || y > 1) ++local_total;    // Not in upper-left
                    if(x < 1 || y > 1) ++local_total;    // Not in upper-right
                    if(x <= 1 && y >= 1) ++local_total;  // In lower-right
                    if(x >= 1 && y >= 1) ++local_total;  // In lower-left
                    break;

                case('2'):
                    if(x <= 1 && y <= 1) ++local_total;  // In upper-left
                    if(x < 1 || y > 1) ++local_total;    // Not in upper-right
                    if(x <= 1 && y >= 1) ++local_total;  // In lower-left
                    if(x >= 1 && y >= 1) ++local_total;  // In lower-right
                    break;

                case('3'):
                    if(x > 1 || y > 1) ++local_total;    // Not in upper-left
                    if(x >= 1 && y <= 1) ++local_total;  // In upper-right
                    if(x > 1 || y < 1) ++local_total;    // Not in lower-left
                    if(x >= 1 && y >= 1) ++local_total;  // In lower-right
                    break;

                case('4'):
                    if(x > 1 || y > 1) ++local_total;    // Not in upper-left
                    if(x < 1 || y > 1) ++local_total;    // Not in upper-right
                    if(x > 1 || y < 1) ++local_total;    // Not in lower-left
                    if(x < 1 || y < 1) ++local_total;    // Not in lower-right
                    break;

                case('5'): 
                    if(x <= 1 && y <= 1) ++local_total;  // In upper-left
                    if(x < 1 || y > 1) ++local_total;    // Not in upper-right
                    if(x <= 1 && y >= 1) ++local_total;  // In lower-left
                    if(x < 1 || y < 1) ++local_total;    // Not in lower-right
                    break;

                case('6'):
                    if(x <= 1 && y <= 1) ++local_total;  // In upper-left
                    if(x >= 1 && y <= 1) ++local_total;  // In upper-right
                    if(x > 1 || y < 1) ++local_total;    // Not in lower-left
                    if(x >= 1 && y >= 1) ++local_total;  // In lower-right
                    break;

                case('7'):
                    if(x > 1 || y > 1) ++local_total;    // Not in upper-left
                    if(x < 1 || y > 1) ++local_total;    // Not in upper-right
                    if(x <= 1 && y >= 1) ++local_total;  // In lower-left
                    if(x >= 1 && y >= 1) ++local_total;  // In lower-right
                    break;

                case('8'): 
                    if(x <= 1 && y <= 1) ++local_total;  // In upper-left
                    if(x >= 1 && y <= 1) ++local_total;  // In upper-right
                    if(x <= 1 && y >= 1) ++local_total;  // In lower-left
                    if(x < 1 || y < 1) ++local_total;    // Not in lower-right
                    break;
            }

            // If the current tile has 0 as a neighbor reduce local total by 1
            if(local_total > 0 && ZeroNeighbor(curr_board, x, y))
                --local_total;

            // Add total calculated for tile to global total 
            total += local_total;
        }
    }

    return total;
}
