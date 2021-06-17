/*
 *    Programmer: Milan Zanussi
 *    Course: Introduction to Artificial Intelligence (CSCI 4350)
 *    Professor: Dr. Joshua Phillips
 *
 *    Project: The ID3 Algorithms for Supervised Learning
 *    Date: November 16th, 2018
 *    File: id3.cpp
 *    File Description: A program which takes as an input two
 *    sets of data arranged with the data points arrange into
 *    rows and the corresponding attributes arranged into
 *    columns, and then tests the ID3 implementation in id3_tree.h
 *    using the data.
 *    
 */

#include <ostream>
#include <fstream>
#include <cassert>
#include <vector>
#include "id3_tree.h"

using namespace std;

// Transpose function: Corrects a fatal error
vector<vector<double>> transpose(vector<vector<double>> matrix);

int main(int argc, char** argv)
{
    // Check for correct number of arguments
    if(argc != 4)
    {
        cerr << "Usage: ./id3 <number of features> <training data file> <test data file>" << endl;
        return -1;
    }

    // Get number of predictors
    unsigned int predictors = atoi(argv[1]);

    // Open files
    ifstream train;
    train.open(argv[2]);
    assert(train);

    ifstream test;
    test.open(argv[3]);
    assert(test);

    // Construct data matrix
    vector<vector<double>> data_matrix;
    vector<int> class_vector;
    vector<double> entry;

    // Read trainging data from file
    unsigned int column = 0;
    double curr_item;
    while( train >> curr_item )
    {
        if(column == predictors)
        {
            // Push entry to data matrix and push class description to class vector
            data_matrix.push_back(entry);
            class_vector.push_back(int(curr_item));
            column = 0;

            // Clear current entry
            entry.clear();
        }

        else
        {
            // Add attribute value to entry
            entry.push_back(curr_item);
            ++column;

        }
    }

    // If the column number is not 0, the attributes do not match the data file
    if(column != 0)
    {
        cerr << "ERROR: Training data did not parse correctly." << endl;
        return -2;
    }

    // Construct ID3 tree
    data_matrix = transpose(data_matrix);
    id3_tree classifier(data_matrix, class_vector);
    //cout << "Ready to classify" << endl;

    // Clear data to make room for test data
    data_matrix.clear();
    class_vector.clear();

    // Collect test data
    while( test >> curr_item )
    {
        if(column == predictors)
        {
            // Push entry to data matrix and push class description to class vector
            data_matrix.push_back(entry);
            class_vector.push_back(int(curr_item));
            column = 0;

            // Clear current entry
            entry.clear();
        }

        else
        {
            // Add attribute value to entry
            entry.push_back(curr_item);
            ++column;
        }
    }

    // If the test data columns mismatch
    if(column != 0)
    {
        cerr << "ERROR: Training data did not parse correctly." << endl;
        return -2;
    }

    // Use ID3 tree on training data
    unsigned int test_correct = 0;
    for(int i = 0; i < class_vector.size(); ++i)
    {
        if( classifier.Classify(data_matrix[i]) == class_vector[i] ) ++test_correct;
    }

    cout << test_correct << endl;
	
    return 0;

}

vector<vector<double>> transpose(vector<vector<double>> matrix)
{
    vector<vector<double>> new_matrix;

    // Assume matrix is not jagged
    for(int j = 0; j < matrix[0].size(); ++j)
    {
        vector<double> new_row;

        for(int i = 0; i < matrix.size(); ++i)
        {
            new_row.push_back(matrix[i][j]);
        }

        new_matrix.push_back(new_row);
    }

    return new_matrix;
}
