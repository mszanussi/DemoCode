/*
 *	Author: Milan Zanussi
 *	Class: CSCI 3110-001
 *	Professor: Dr. Zhijiang Dong
 *	Assignment: Open Lab 5 -- Binary Tree Level Sum
 *	Description: Write a program that will read a series
 *	of binary trees from a file and output information
 *	corresponding to each tree (such as node count and
 *	the sum of the depths of each node).
 */
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <stack>
#include <cmath>
#include <iomanip>

using namespace std;

#include "BST.h"

//The name of the file containing binary search trees
const string    FILENAME = "bst.dat";

//The value indicating the end of a binary search tree
// in the file FileName
const int       FILE_SENTINEL = -1;

int main( void )
{

	ifstream treeFile;						// File stream for tree data
	BinarySearchTree<int> levelCountTree;	// Tree to store numbers read from file
	int currentRead;						// Current number read from file
	int nodeCount = 0;						// Current Node
	int treeNumber = 0;						// Current tree number
	int levelTotal;							// Stores the total level of current tree
	double logOutput;						// Stores the output of log2(n)

	// Read file specified by the FILENAME constant
	// and check if the file exists
	treeFile.open(FILENAME.c_str());
	assert(treeFile);

	// Loop until the entire file is read
	while (treeFile >> currentRead) {
		
		// If the current read contains an item
		// to store into the tree
		if (currentRead != FILE_SENTINEL) {

			// Insert integer to tree
			levelCountTree.searchTreeInsert(currentRead);
			// Increment node count
			++nodeCount;

		}	// END IF

		// Otherwise, all tree info has been read
		// and it may be evaluated and output to the screen
		else {

			// Set variables to store result to save computation time
			levelTotal = levelCountTree.totalLevel();
			logOutput = log2(nodeCount);

			// Print details about tree with leading line for separation
			cout << "-----------------------------------------------" << endl
				<< "Tree No. " << ++treeNumber << endl
				<< "Number of Nodes: " << nodeCount << endl
				<< "Total Level: " << levelTotal << endl
				<< "log2(n) of Node Count: " << setprecision(10) << logOutput << endl
				<< "Average Node Level: " << ((double)levelTotal) / nodeCount << endl
				<< "Average Level divided by log2(n): " << ((double)levelTotal) / (logOutput * nodeCount)
				<< endl;

			// Clear tree data for next data set
			levelCountTree = BinarySearchTree<int>();
			nodeCount = 0;

		}	// END ELSE
	}

	// Print trailing line to finish program
	cout << "-----------------------------------------------" << endl;

	// Close file after usage
	treeFile.close();

    return 0;

}	// END MAIN
