/*
* Author: Milan Zanussi
* File: "main.cpp"
* Date: October 22nd, 2016
*
* Description: This file tests the runtime of four sorting algorithms
* on three cases of arrays (random, sorted, and inverse order) and 
* outputs runtime data to a file called "result.dat"
*/

#include <iostream>
#include "algorithm.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cassert>
#include <iomanip>

// Number of different array sizes being sorted
const int SIZE_COUNT = 15;

// Number of algorithms to test
const int ALGO_COUNT = 4;

using namespace std;

//*********|| MISCELLANEOUS FUNCTIONS ||*********\\

//	CopyArray:
//		Copies the contents of one array to another
template<typename DataType>
void CopyArray(DataType srcArray[], DataType copyArray[], int size);

//	GenerateRandomArray:
//		Generates random integers and stores them in an array
void GenerateRandomArray(int array[], int size);

//	OutputTestHeader:
//		Outputs labels for all functions onto the ostream
//		passed as the parameter
void OutputTestHeader(ostream& os);

//	OutputTestRow:
//		Outputs all information related to each size and the time
//		it took for the algorithm to sort the array of that size
void OutputTestRow(int size, clock_t compTimes[], ostream& os);

//************|| TESTING FUNCTIONS ||************\\

//	TestBubbleSort:
//		Returns the clock time it took for the program
//		to bubble sort a given array.
template<typename DataType>
clock_t TestBubbleSort(DataType array[], int size);

//	TestInsertionSort:
//		Returns the clock time it took for the program
//		to perform an inertion sort on a given array.
template<typename DataType>
clock_t TestInsertionSort(DataType array[], int size);

//	TestMergeSort:
//		Returns the clock time it took for the program
//		to merge sort a given array
template<typename DataType>
clock_t TestMergeSort(DataType array[], int size);

//	TestQuickSort:
//		Returns the clock time it took for the program
//		to quick sort a given array
template<typename DataType>
clock_t TestQuickSort(DataType array[], int size);

//	TestRandomEntries:
//		Specific to this program, generates random arrays
//		and passes them through each sorting algorithm
//		with only size as a parameter
void TestRandomEntries(int size, clock_t timeArray[]);

//	TestSortedEntries:
//		Specific to this program, generates random arrays
//		and passes them through each sorting algorithm
//		with only size as a parameter
void TestSortedEntries(int size, clock_t timeArray[]);

//	TestInverseEntries:
//		Specific to this program, generates random arrays
//		and passes them through each sorting algorithm
//		with only size as a parameter
void TestInverseEntries(int size, clock_t timeArray[]);

int main()
{
	// File for runtime results
	ofstream runtimeResults;

	// Open file and assert
	runtimeResults.open("results.dat");
	assert(runtimeResults);

	// Computation time for each sorting algorithm
	// 0: Bubble, 1: Insertion, 2: Merge, 3: Quick
	clock_t compTimes[ALGO_COUNT];

	// Initialize array of all test sizes
	int sizeArray[SIZE_COUNT] = { 10, 50, 100, 200, 300, 400,
									500, 1000, 2000, 3000, 4000,
									5000, 10000, 50000, 100000 };

	// Variable that stores what size to use for each iteration
	// of the test
	int useSize;

	// RANDOM CASES:

	// Leading header for random cases
	cout << "Sorting Test on Random Arrays:" << endl << endl;
	runtimeResults << "Sorting Test on Random Arrays:" << endl << endl;

	// Output headers to console and results file
	OutputTestHeader(cout);
	OutputTestHeader(runtimeResults);

	// Test randomly-organized cases for all sizes
	for (int i = 0; i < SIZE_COUNT; ++i) {

		// Set useSize for iteration
		useSize = sizeArray[i];

		// Test all algorithms for current size specification
		TestRandomEntries(useSize, compTimes);

		// Output results to console and file
		OutputTestRow(useSize, compTimes, cout);
		OutputTestRow(useSize, compTimes, runtimeResults);

	}

	// SORTED CASES:

	// Leading header for sorted cases
	cout << endl << "Sorting Test on Sorted Arrays:" << endl << endl;
	runtimeResults << endl << "Sorting Test on Sorted Arrays:" << endl << endl;

	// Output headers to console and results file
	OutputTestHeader(cout);
	OutputTestHeader(runtimeResults);

	// Test randomly-organized cases for all sizes
	for (int i = 0; i < SIZE_COUNT; ++i) {

		// Set useSize for iteration
		useSize = sizeArray[i];

		// Test all algorithms for current size specification
		TestSortedEntries(useSize, compTimes);

		// Output results to console and file
		OutputTestRow(useSize, compTimes, cout);
		OutputTestRow(useSize, compTimes, runtimeResults);
	}

	// INVERSE CASES:

	// Leading header for sorted cases
	cout << endl << "Sorting Test on Inverse Arrays:" << endl << endl;
	runtimeResults << endl << "Sorting Test on Inverse Arrays:" << endl << endl;

	// Output headers to console and results file
	OutputTestHeader(cout);
	OutputTestHeader(runtimeResults);

	// Test randomly-organized cases for all sizes
	for (int i = 0; i < SIZE_COUNT; ++i) {

		// Set useSize for iteration
		useSize = sizeArray[i];

		// Test all algorithms for current size specification
		TestInverseEntries(useSize, compTimes);

		// Output results to console and file
		OutputTestRow(useSize, compTimes, cout);
		OutputTestRow(useSize, compTimes, runtimeResults);

	}

	// Close runtime results file
	runtimeResults.close();

	return 0;
}

//	CopyArray:
//		Copies the contents of one array to another
template<typename DataType>
void CopyArray(DataType srcArray[], DataType copyArray[], int size)
{
	// For each item in the srcArray, copy it to the copyArray
	for (int i = 0; i < size; ++i) {
		copyArray[i] = srcArray[i];
	}
}

//	GenerateRandomArray:
//		Generates random integers and stores them in an array
void GenerateRandomArray(int intArray[], int size)
{
	// Seeds the random number generator
	srand((unsigned)time(0));

	// For each index in the array, fill it with a random number
	for (int i = 0; i < size; ++i) {
		intArray[i] = rand();
	}

	return;
}

//	OutputTestHeader:
//		Outputs labels for all functions onto the ostream
//		passed as the parameter
void OutputTestHeader(ostream& os) {

	// Output all headers in the following format the the screen
	os << left << setw(15) << "ARRAY SIZE:"
		<< setw(15) << "BUBBLE:"
		<< setw(15) << "INSERTION:"
		<< setw(15) << "MERGE:"
		<< setw(15) << "QUICK:"
		<< endl << endl;
}

//	OutputTestRow:
//		Outputs all information related to each size and the time
//		it took for the algorithm to sort the array of that size
void OutputTestRow(int size, clock_t compTimes[], ostream& os)
{
	// Output all headers in the following format the the screen
	os << left << setw(15) << size
		<< setw(15) << compTimes[0]
		<< setw(15) << compTimes[1]
		<< setw(15) << compTimes[2]
		<< setw(15) << compTimes[3]
		<< endl;
}

//	TestBubbleSort:
//		Returns the clock time it took for the program
//		to bubble sort a given array.
template<typename DataType>
clock_t TestBubbleSort(DataType userArray[], int size)
{
	// Computation time to return
	clock_t compTime;

	// Take clock count before operation
	compTime = clock();

	// Call bubble sort
	bubbleSort(userArray, size);

	// Take afterward clock count minus previous clock count for total time
	// and convert to seconds and then miliseconds
	compTime = (clock() - compTime);

	return compTime;
}

//	TestInsertionSort:
//		Returns the clock time it took for the program
//		to perform an inertion sort on a given array.
template<typename DataType>
clock_t TestInsertionSort(DataType userArray[], int size)
{
	// Computation time to return
	clock_t compTime;

	// Take clock count before operation
	compTime = clock();

	// Call insertion sort
	insertionSort(userArray, size);

	// Take afterward clock count minus previous clock count for total time
	// and convert to seconds and then miliseconds
	compTime = (clock() - compTime);

	return compTime;
}

//	TestMergeSort:
//		Returns the clock time it took for the program
//		to merge sort a given array
template<typename DataType>
clock_t TestMergeSort(DataType userArray[], int size)
{
	// Computation time to return
	clock_t compTime;

	// Take clock count before operation
	compTime = clock();

	// Call merge sort
	mergeSort(userArray, 0, size-1, size);

	// Take afterward clock count minus previous clock count for total time
	// and convert to seconds and then miliseconds
	compTime = (clock() - compTime);

	return compTime;
}

//	TestQuickSort:
//		Returns the clock time it took for the program
//		to quick sort a given array
template<typename DataType>
clock_t TestQuickSort(DataType userArray[], int size)
{
	// Computation time to return
	clock_t compTime;

	// Take clock count before operation
	compTime = clock();

	// Call bubble sort
	quickSort(userArray, 0, size-1);

	// Take afterward clock count minus previous clock count for total time
	// and convert to seconds
	compTime = (clock() - compTime);

	return compTime;
}

void TestRandomEntries(int size, clock_t timeArray[]) {

	// Dynamically allocate array for memory conservation
	int *testArray = new int[size];

	// Generate random array to sort
	GenerateRandomArray(testArray, size);

	// Create a copyArray so all functions sort the same data
	int *copyArray = new int[size];

	// Test each algorithm at sorting random arrays
	for (int j = 0; j < 4; ++j) {

		CopyArray<int>(testArray, copyArray, size);

		switch (j) {

			// Bubble Sort case
		case(0) :
			timeArray[0] = TestBubbleSort(copyArray, size);
			break;

			// Insertion sort case
		case(1) :
			timeArray[1] = TestInsertionSort(copyArray, size);
			break;

			// Merge sort case
		case(2) :
			timeArray[2] = TestMergeSort(copyArray, size);
			break;

			// Quick Sort case
		case(3) :
			timeArray[3] = TestQuickSort(copyArray, size);
			break;
		}
	}

	// Deallocate memory at arrays
	delete testArray;
	delete copyArray;

	return;
}

void TestSortedEntries(int size, clock_t timeArray[]) {

	// Dynamically allocate array for memory conservation
	int* testArray = new int[size];

	// Generate an array of items in the proper order
	for (int i = 0; i < size; ++i) {
		testArray[i] = i;
	}

	// Test each algorithm at sorting random arrays
	for (int j = 0; j < 4; ++j) {

		switch (j) {

			// Bubble Sort case
		case(0) :
			timeArray[0] = TestBubbleSort(testArray, size);
			break;

			// Insertion sort case
		case(1) :
			timeArray[1] = TestInsertionSort(testArray, size);
			break;

			// Merge sort case
		case(2) :
			timeArray[2] = TestMergeSort(testArray, size);
			break;

			// Quick Sort case
		case(3) :
			timeArray[3] = TestQuickSort(testArray, size);
			break;
		}
	}

	// Deallocate memory at array
	delete testArray;

	return;
}

void TestInverseEntries(int size, clock_t timeArray[]) {
	// Dynamically allocate array for memory conservation
	int* testArray = new int[size];

	// Test each algorithm at sorting random arrays
	for (int j = 0; j < 4; ++j) {

		// Generate an array of items in the proper order
		for (int i = 0; i < size; ++i) {
			testArray[i] = (size - i);
		}

		switch (j) {

			// Bubble Sort case
		case(0) :
			timeArray[0] = TestBubbleSort(testArray, size);
			break;

			// Insertion sort case
		case(1) :
			timeArray[1] = TestInsertionSort(testArray, size);
			break;

			// Merge sort case
		case(2) :
			timeArray[2] = TestMergeSort(testArray, size);
			break;

			// Quick Sort case
		case(3) :
			timeArray[3] = TestQuickSort(testArray, size);
			break;
		}
	}

	// Deallocate memory at array
	delete testArray;

	return;
}