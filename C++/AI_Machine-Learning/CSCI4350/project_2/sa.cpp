/*
 *    Programmer: Milan Zanussi
 *    Course: Introduction to Artificial Intelligence (CSCI 4350)
 *    Professor: Dr. Joshua Phillips
 *
 *    Project: A Comparison of Local Search Strategies
 *    Date: October 25th, 2018
 *    File: sa.cpp
 *    File Description: This file implements simulated
 *    annealing using the Metropolis criterion for
 *    choosing decremental moves to find a local
 *    maximum on a sum of Gaussian random varaibles.
 *
 */

#include <iostream>
#include <cstdlib>
#include <cmath>

#define EPSILON   0.00000001  // Error tolerance for convergence
#define STEP_SIZE 0.1        // Step size of greedy hill climb
#define INV_STEP  10         // Reciporical of step size for computing divergence
#define ANNEAL    7510.0     // An annealing constant for giving a base temperature

#include "SumofGaussians.h"

using namespace std;

// Laplacian: Computes the Laplacian ( divergence of the gradient )
// of a vector along the sum of Gaussians function using forward
// difference approximation
double Laplacian(double vector[], int dimensions, SumofGaussians gauss_sum);

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

    // Compute randomized starting point
    for(int i = 0; i < dimens; ++i)
    {
        // Generate random components on interval [0,10]
        preimage[i] = ((1.0*rand())/RAND_MAX)*10.0;
    }

    // Perform simulated annealing to find maximum
    for(int i = 1; i < 7501; ++i)
    {
        // Variables to store some terms to speed up computation
        double curr_val, next_val;

        // Compute the Laplacian of the sum at the current preimage
        // to get a measurement of "coolness" and proximity to
        // points which are relatively more "warm"
        double coolness = 0.0;

        // Compute gradient
        sum_func.deriv(preimage, grad);

        // Compute current function value
        curr_val = sum_func.eval(preimage);

        // Approximate the second dervatives using gradient
        // and sum them up into the divergence
        for(int x = 0; x < dimens; ++x)
        {
            double forward[dimens];
        
            for(int y = 0; y < dimens; ++y)
            {
                // Only apply the forward step in one direction to take
                // second partial derivative in that direction
                if (x == y)    forward[y] = preimage[y] + STEP_SIZE;
                else           forward[y] = preimage[y];
            }

            coolness += sum_func.eval(forward) - curr_val + STEP_SIZE*grad[x];
        }

        // Multiply sum by reciporical of STEP_SIZE squared and 2 to finish
        // divergence approximation
        // coolness = (2*coolness);
        coolness = (2*coolness)*(INV_STEP*INV_STEP);
		
		//coolness = Laplacian(grad,dimens,sum_func);

        // Compute temperature using coolness
        //double temperature = ANNEAL/(i*(1.0 + ((coolness > 0.0) ? coolness : 0.0)));
        double temperature = (ANNEAL - i)/((coolness > 0.0) ? coolness : 1.0);

        // Print the current preimage
        //for(int j = 0; j < dimens; ++j)
        //{   cout << preimage[j] << " ";   }
        cout << curr_val << endl;

        // Choose random next step around preimage
        double next_preimage[dimens];
        for(int j = 0; j < dimens; ++j)
        {
            // Let next_preimage be preimage with each component preturbed by
            // some random float value in the interval [-0.1,0.1]
            next_preimage[j] = preimage[j] + ((STEP_SIZE*rand())/(RAND_MAX))*((rand()%2 == 0)? 1.0 : -1.0);
        }

        // Determine whether the next preimage should be used following
        // the metropolis criterion
        bool move = false;
        next_val = sum_func.eval(next_preimage);
        if(next_val > curr_val)
            move = true;

        else
        {
            if(double(rand())/double(RAND_MAX) < (next_val - curr_val)/temperature)
                move = true;
        }

        // If the move was chosen
        if(move)
        {
            // Set the preimage equal to the new preimage
            for(int k = 0; k < dimens; ++k)
            {
                preimage[k] = next_preimage[k];
            }
        }

    }
 
    // Print the final results
    for(int l = 0; l < dimens; ++l)
        cout << preimage[l] << " ";
    cout << sum_func.eval(preimage) << endl;

    return 0;
}

// Laplacian: Computes the Laplacian ( divergence of the gradient )
// of a vector along the sum of Gaussians function using forward
// difference approximation
double Laplacian(double vector[], int dimensions, SumofGaussians gauss_sum)
{
    // Contains divergence of gradient
    double divergence = 0.0;
    double grad[dimensions];

    // Compute gradient
    gauss_sum.deriv(vector, grad);

    // Approximate the second dervatives using gradient
    // and sum them up into the divergence
    for(int i = 0; i < dimensions; ++i)
    {
        double forward[dimensions];
        
        for(int j = 0; j < dimensions; ++j)
        {
            // Only apply the forward step in one direction to take
            // second partial derivative in that direction
            if (i == j)    forward[j] = vector[j] + STEP_SIZE;
            else           forward[j] = vector[j];
        }

        divergence += gauss_sum.eval(forward) - gauss_sum.eval(vector) - STEP_SIZE*grad[i];
    }

    // Multiply sum by reciporical of STEP_SIZE squared and 2 to finish
    // divergence approximation
    divergence = 2*divergence*(INV_STEP*INV_STEP);

    return divergence;

}
