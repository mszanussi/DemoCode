/*
 *    Programmer: Milan Zanussi
 *    Course: Introduction to Artificial Intelligence (CSCI 4350)
 *    Professor: Dr. Joshua Phillips
 *
 *    Project: A Comparison of Local Search Strategies
 *    Date: October 25th, 2018
 *    File: greedy.cpp
 *    File Description: This file implements a greedy
 *    hill-climbing procedure (gradient ascent) to 
 *    find a local maximum on a sum of Gaussian
 *    random varaibles.
 *
 */

#include <iostream>
#include <cstdlib>
#include <cmath>

#define EPSILON 0.00000001    // Error tolerance for convergence
#define STEP_SIZE 0.01        // Step size of greedy hill climb

#include "SumofGaussians.h"

using namespace std;

// VectorNorm: Takes the Euclidean norm of a vector
// with a specified number of dimensions
double VectorNorm(double vector[], int dimensions);

// ScaleVector: Scales a vector by a constant value
void ScaleVector(double vector[], int dimensions, double scalar);

// Mainline logic
int main(int argc, char** argv)
{
    // Variables for storing the number of summands and dimensions
    int dimens, summands;

    // Check command line arguments for errors
    if (argc != 4)
    {
        cout << "Usage: ./greedy <random number seed> <dimensions> <number of random variables>" << endl;
        return 1;
    }

    // Read command line arguments into variables
    dimens = atoi(argv[2]);
    summands = atoi(argv[3]);

    // Seed random number generator with command line argument
    srand(atoi(argv[1]));

    // Compute a sum of Gaussians
    SumofGaussians sum_func(dimens, summands);

    // Variables to store preimage and gradient of preimage
    double preimage[dimens], grad[dimens];

    // Generate random initial point
    for(int i = 0; i < dimens; ++i)
    {
        // Generate random components on interval [0,10]
        preimage[i] = ((1.0*rand())/RAND_MAX)*10.0;
    }

    // Find an initial gradient
    sum_func.deriv(preimage, grad);

    // Scale gradient to appropriate step size
    ScaleVector(grad,dimens,STEP_SIZE);

    // Perform greedy local search on sum of Gaussians
    while( VectorNorm(grad, dimens) >= EPSILON )
    {
        // Print current preimage and function value
        for(int j = 0; j < dimens; ++j)
            cout << preimage[j] << " ";
        cout << " " << sum_func.eval(preimage) << endl;

        // Recompute preimage by following gradient
        for(int k = 0; k < dimens; ++k)
        {
            preimage[k] = preimage[k] + grad[k];
        }

        // Recompute gradient given change in preimage
        sum_func.deriv(preimage, grad);
        ScaleVector(grad,dimens,STEP_SIZE);
    }

    // Print terminal values
    for(int j = 0; j < dimens; ++j)
        cout << preimage[j] << " ";
    cout << " " << sum_func.eval(preimage) << endl;

    return 0;

}

// VectorNorm: Takes the Euclidean norm of a vector
// with a specified number of dimensions
double VectorNorm(double vector[], int dimensions)
{
    // Initialiaze norm to 0
    double norm = 0.0;

    // Take sum of squares of components
    for(int i = 0; i < dimensions; ++i)
    {
        norm += vector[i]*vector[i];
    }

    // Return square room of sum of squares
    return sqrt(norm);
}

// ScaleVector: Scales a vector by a constant value
void ScaleVector(double vector[], int dimensions, double scalar)
{
    for(int i = 0; i < dimensions; ++i)
        vector[i] = vector[i]*scalar;
}
