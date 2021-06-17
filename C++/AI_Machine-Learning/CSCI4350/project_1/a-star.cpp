/*
	Author: Milan Zanussi
	Class: CSCI 4350: Intro to Artificial Intelligence
	Project: Project 1: A* Search and 8-Slider Puzzle
	File: a-star.cpp
	Description: Program which implements the A* search
	algorithm with different heuristics specified in heuristics.h
	which will be used to solve the 8-tile slider puzzle.
*/

#include <iostream>     // For standard input and output
#include <cmath>        // For logarithm functions
#include <string>       // For means of ordering configurations
#include <map>          // For keeping closed list
#include <queue>        // For maintaining priority queue frontier

// ALL HEURISTIC FUNCTIONS ARE LOCATED
// IN THIS FILE:
#include "heuristics.h"

using namespace std;

// Solution data which will be used to determine the
// efficacy of some heuristics over others.
struct SolutionData
{
    unsigned int expanded_nodes;
    unsigned int total_nodes;
    unsigned int solution_depth;
    double approx_branching;

    // Sequence of all states of the puzzle from
    // initial state to the goal. Each state is
    // separated by a blank line.
    string state_sequence;
};

// Node struct for use in AStar stores a board
// configuration, a node ID, and a path to goal cost.
struct BoardNode
{
    string parent_config;
    string configuration;
    unsigned int node_id;
    int path_cost;
    int approx_total_cost;

};

// Comparison operator for board nodes
bool operator<(const BoardNode& lhs, const BoardNode& rhs)
{
    // Path weight costs less
    if(lhs.approx_total_cost > rhs.approx_total_cost)
        return true;

    // Path weight is equal but left-hand node was created first
    else if((lhs.approx_total_cost == rhs.approx_total_cost) && (lhs.node_id > rhs.node_id))
        return true;

    // Otherwise
    else
        return false;
}

// Function for swapping characters by reference
// Used in 8-puzzle A* function
void SwapChar(char &a, char &b)
{
    char temp = a;
    a = b;
    b = temp;

    return;
}

// AStar: A* search algorithm which takes a heuristic, a board configuration, and
// a goal state for the board reconfiguration, and attempts to solve the 8-puzzle
// using the given heuristic.
SolutionData AStar(int (*heuristic)(char (*)[3]), char board[][3]);

// Mainline logic of applying A* to a particular board configuration
// with a user-specified heuristic
int main(int argc, char** argv)
{
    char board[3][3];    // Initial configuration of board
    char option;         // Indicator of which heuristic to use 

    int (*heuristic)(char (*)[3]);    // Pointer to heuristic to use as input

    // Print usage message on argument error
    if(argc != 2)
    {
        cerr << "Usage: astar <heuristic option>" << endl
             << "The allowed heuristic options are:" << endl
             << "    0 -- Uniform Cost Search" << endl
             << "    1 -- Displacement Heuristic" << endl
             << "    2 -- Taxicab Heuristic" << endl
             << "    3 -- Inclusion Heuristic" << endl;

        return -1;
    }

    // Read option of command line
    option = argv[1][0];

    // Check if the heuristic option is valid
    if(option < '0' || option > '3')
    {
        cerr << "The heuristic option specified is not recognized." << endl;
        return -2;
    }

    // Take initial board input
    for(int j = 0; j < 3; ++j)
    {
        for(int i = 0; i < 3; ++i)
        {
            cin >> board[i][j];
        }
    }

    // Decide which heuristic to use
    switch(option)
    {
        case('0'): heuristic = UniformHeuristic;   break;    // Uniform Cost Search
        case('1'): heuristic = DisplaceHeuristic;  break;    // Displacement Search
        case('2'): heuristic = TaxicabHeuristic;   break;    // Taxicab Search
        case('3'): heuristic = InclusionHeuristic; break;    // Corner Inclusion Search
    }

    // Compile data on heuristic function applied to specified board
    SolutionData test_data;
    test_data = AStar(heuristic, board);

    // Print data to standard output
    cout << "V=" << test_data.expanded_nodes << endl
         << "N=" << test_data.total_nodes << endl
         << "d=" << test_data.solution_depth << endl
         << "b=" << test_data.approx_branching << endl << endl
         << endl << test_data.state_sequence << endl;

    return 0;
}

// BoardToString: Converts a board configuration into linear string
string BoardToString(const char board[][3])
{
    string conversion = "";

    for(int j = 0; j < 3; ++j)
        for(int i = 0; i < 3; ++i)
            conversion += board[i][j];

    return conversion;
}

// AStar: A* search algorithm which takes a heuristic, a board configuration, and
// a goal state for the board reconfiguration, and attempts to solve the 8-puzzle
// using the given heuristic.
SolutionData AStar(int (*heuristic)(char (*)[3]), char board[][3])
{
    // A set of closed configurations with previous configuration
    map<string,string> closed;

    // A priority queue of board nodes
    priority_queue<BoardNode> frontier;

    // Location of zero
    int zero_x,zero_y;

    // Solution data for current trial
    SolutionData results;
    results.state_sequence = "";
    results.total_nodes = 1;        // This includes initial node
    results.expanded_nodes = 0;

    // Create initial node
    BoardNode new_node;
    new_node.configuration = BoardToString(board);
    new_node.parent_config = "";
    new_node.node_id = 0;
    new_node.path_cost = 0;
    new_node.approx_total_cost = (*heuristic)(board);

    // Add initial to frontier
    frontier.push(new_node);

    // Begin search
    bool goal_found = false;
    while(!frontier.empty())
    {
        // Pull most preferred node from frontier
        BoardNode head_node;
        head_node = frontier.top();
        frontier.pop();
        ++results.expanded_nodes;

        // Create temporary borad and let it be equal to string representation
        // of head node, and find the zero on the board
        char temp_board[3][3];
        for(int k = 0; k < head_node.configuration.size(); ++k)
        {
            temp_board[k%3][k/3] = head_node.configuration[k];

            if(head_node.configuration[k] == '0')
            {
                zero_x = k%3;
                zero_y = k/3;
            }
        }

        // Check if the current node is the goal
        bool is_goal = true;
        for(int k = 0; k < 9; ++k)
        {
            // If there is any discrepancy from the goal state
            // (goal state is all numbers in order)
            if(k + '0' != head_node.configuration[k])
            {
                is_goal = false;
                break;
            }
        }

        // If the expanded node was the goal, break search
        if(is_goal)
        {
            goal_found = true;

            // Set solution depth to path cost to goal
            results.solution_depth = head_node.path_cost;
            
            // Put goal state on closed list
            closed.insert(pair<string,string>(head_node.configuration, head_node.parent_config));

            // Put configuration in new_node to export data from loop
            new_node.configuration = head_node.configuration;

            break;
        }

        // If the expanded node was not the goal, consider
        // all of the possible configurations adjacent to it.

        // Expand left if possible
        if(zero_x - 1 >= 0)
        {
            // Swap zero with char to left
            SwapChar(temp_board[zero_x][zero_y], temp_board[zero_x - 1][zero_y]);

            // Reuse new_node to make new node
            new_node.configuration = BoardToString(temp_board);
 
            // If configuration is not closed, finish constructing the node
            if( closed.find(new_node.configuration) == closed.end() )
            {
                new_node.parent_config = head_node.configuration;
                new_node.node_id = results.total_nodes++;
                new_node.path_cost = head_node.path_cost + 1;
                new_node.approx_total_cost = new_node.path_cost + (*heuristic)(temp_board);

                // Add new node to frontier
                frontier.push(new_node);
            }

            // Swap back to previous configuration
            SwapChar(temp_board[zero_x][zero_y], temp_board[zero_x - 1][zero_y]);
        }

        // Expand right if possible
        if(zero_x + 1 <= 2)
        {
            // Swap zero with char to left
            SwapChar(temp_board[zero_x][zero_y], temp_board[zero_x + 1][zero_y]);

            // Reuse new_node to make new node
            new_node.configuration = BoardToString(temp_board);

            // If configuration is not closed, finish constructing the node
            if( closed.find(new_node.configuration) == closed.end()  )
            {
                new_node.parent_config = head_node.configuration;
                new_node.node_id = results.total_nodes++;
                new_node.path_cost = head_node.path_cost + 1;
                new_node.approx_total_cost = new_node.path_cost + (*heuristic)(temp_board);

                // Add new node to frontier
                frontier.push(new_node);
            }

            // Swap back to original configuration
            SwapChar(temp_board[zero_x][zero_y], temp_board[zero_x + 1][zero_y]);

        }

        // Expand below if possible
        if(zero_y - 1 >= 0)
        {
            // Swap zero with char to left
            SwapChar(temp_board[zero_x][zero_y], temp_board[zero_x][zero_y - 1]);

            // Reuse new_node to make new node
            new_node.configuration = BoardToString(temp_board);

            // If configuration is not closed, finish constructing the node
            if( closed.find(new_node.configuration) == closed.end() )
            {
                new_node.parent_config = head_node.configuration;
                new_node.node_id = results.total_nodes++;
                new_node.path_cost = head_node.path_cost + 1;
                new_node.approx_total_cost = new_node.path_cost + (*heuristic)(temp_board);

                // Add new node to frontier
                frontier.push(new_node);
            }

            // Swap back to original configuration
            SwapChar(temp_board[zero_x][zero_y], temp_board[zero_x][zero_y - 1]);
        }

        // Expand above if possible
        if(zero_y + 1 <= 2)
        {
            // Swap zero with char to left
            SwapChar(temp_board[zero_x][zero_y], temp_board[zero_x][zero_y + 1]);

            // Reuse new_node to make new node
            new_node.configuration = BoardToString(temp_board);

            // If configuration is not closed, finish constructing the node
            if( closed.find(new_node.configuration) == closed.end() )
            {
                new_node.parent_config = head_node.configuration;
                new_node.node_id = results.total_nodes++;
                new_node.path_cost = head_node.path_cost + 1;
                new_node.approx_total_cost = new_node.path_cost + (*heuristic)(temp_board);

                // Add new node to frontier
                frontier.push(new_node);
            }

            // Swap back to original configuration
            SwapChar(temp_board[zero_x][zero_y], temp_board[zero_x][zero_y + 1]);
        }

        // Add expanded node to the closed list
        closed.insert(pair<string,string>(head_node.configuration, head_node.parent_config));
    }

    // Bad puzzle, sad puzzle
    if(!goal_found)
        cout << "Puzzle could not be solved." << endl;

    // Add all nodes on found path to state sequence
    string curr_state = new_node.configuration;
    while( curr_state != "" && goal_found)
    {
        // Add state in board form to front of sequence
        for(int k = 8; k >=0 ; --k)
        {
            // First item of each row is preceeded by newline from previous row
            if(k%3 == 0)
                results.state_sequence = "\n" + (curr_state[k] + results.state_sequence);

            // Space between each row entry
            else 
                results.state_sequence = " " + (curr_state[k] + results.state_sequence);
        }

        // Lookup previous state of current state and let it be the new current state
        curr_state = closed[curr_state];

        // Add extra spacing before each state except for the first state
        if (curr_state != "")
            results.state_sequence = "\n" + results.state_sequence;

    }

    // Erase leading newline from the state sequence
    results.state_sequence.erase(0,1);

    // Calculate approximate branching factor using logarithms
    results.approx_branching = pow(double(results.total_nodes), 1.0/double(results.solution_depth));

    return results;
}
