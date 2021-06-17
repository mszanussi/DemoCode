
#include <iostream>
#include <fstream>
#include <cassert>
#include "kmeans_class.h"

using namespace std;

int main(int argc, char** argv)
{

    // Variables for program
    int seed;
    unsigned int centroids, features, p;
    ifstream train, test;

    // Check if argument count is correct
    if(argc < 6 || argc > 7)
    {
        cerr << "Usage: ./kmeans <random number seed> <centroids> <data features> <training data> <testing data>" << endl;
        return -10;
    }

    if ( argc == 7 )
    {
        // If an optional 7th argument was given
        // it must be the p value for the Lp norm
        p = atoi(argv[6]);
    }

    // Assign arguments to correct location
    seed = atoi(argv[1]);
    centroids = atoi(argv[2]);
    features = atoi(argv[3]);
    p = 2;

    // If there was a seventh argument, that was the 
    if ( argc == 7 )
    {
        // If an optional 7th argument was given
        // it must be the p value for the Lp norm
        p = atoi(argv[6]);
    }

    // Try to open test data file
    train.open(argv[4]);
    assert(train);

    // Construct data matrix
    vector<vector<double>> data_matrix;
    vector<int> class_vector;
    vector<double> entry;

    // Read training data from file
    unsigned int column = 0;
    double curr_item;
    while( train >> curr_item )
    {
        if(column == features)
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

    // Discharge training data file
    train.close();

    // If the column number is not 0, the attributes do not match the data file
    if(column != 0)
    {
        cerr << "ERROR: Features count does not match the number of entries in the training data file." << endl;
        return -11;
    }

    // Construct kmeans classifier
    //cout << "Constructing kmeans classifier with p = "<< p << endl;
    kmeans classifier(seed,centroids,features,data_matrix,class_vector,p);

    // Open test data file
    test.open(argv[5]);
    assert(test);

    // Read test data reusing the data matrix
    data_matrix.clear();
    class_vector.clear();
    column = 0;
    while( test >> curr_item )
    {
        if(column == features)
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

    // Discharge test data file
    test.close();

    // If the column number is not 0, the attributes do not match the data file
    if(column != 0)
    {
        cerr << "ERROR: Features count does not match the number of entries in the testing data file." << endl;
        return -12;
    }

    // Test number of correct guesses the classifer makes
    //cout << "Testing classifier" << endl;
    unsigned int correct = 0;
    for(unsigned int s = 0; s < data_matrix.size(); ++s)
        if(classifier.Classify(data_matrix[s]) == class_vector[s])
            ++correct;

    cout << correct << endl;

    return 0;
}
