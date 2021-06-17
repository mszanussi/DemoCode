/*
 * Author: Milan Zanussi
 * File: "algorithm.h"
 * Date: October 22nd, 2016
 *
 * Description: Header file and implementation
 * for different sorting algorithms discussed
 * in the Algorithm and Data Structures class.
 *
 * NOTE: Quicksort and Mergesort related algorithms
 * were programmed by Dr. Zhijiang Dong
 */

using namespace std;

// Declaration of array bubble sort algorithm
template<typename DataType>
void bubbleSort(DataType theArray[], int n)
{
	// Bool to determine if the array
	// is already sorted
	bool sorted = false;

	// For every item in the array while the array
	// is not sorted
	for (int i = 0; (i < n) && (sorted == false); ++i)
	{
		// Assume array is sorted until proven false
		sorted = true;

		// For every item not in the proper place
		for (int j = 0; j < n-i-1; ++j)
		{
			// If the current item is greater than the next
			if (theArray[j] > theArray[j + 1]) {
				//Swap the current item with the next
				swap<DataType>(theArray[j], theArray[j + 1]);
				sorted = false;
			}
		}
	}

	return;
}

// Declaration of array insertion sort algorithm
template<typename DataType>
void insertionSort(DataType theArray[], int n)
{
	int unsortedBegin;		// Index of where unsorted section begins
	bool sorted = false;	// Bool of whether the array is already sorted

	// Find the beginning of unsorted data section
	for (int i = 1; i < n; ++i)
	{
		// If the current data is out of order
		if (theArray[i] < theArray[i - 1]) {
			unsortedBegin = i;
			break;
		}

		// If the array is all sorted
		sorted = true;
	}

	// If the array is not sorted, sort it
	if (!sorted){

		DataType temp;	// Temporarily stores data for data shift
		//bool shiftingData;

		// For every item still unsorted
		for (int j = unsortedBegin; j < n; j = (++unsortedBegin)) {

			// For every index in the sorted section
			for (int k = 0; k < unsortedBegin; ++k){ 

				// If the item finds its proper location
				if (theArray[j] < theArray[k])

					// Swap the items and find theArray[k]'s
					// new location (shift everything over)
					swap<DataType>(theArray[k], theArray[j]);
			}
		}
	}

	return;
}

//******* MERGESORT SECTION *********\\

// Declaration of array merge sort algorithm
template<typename DataType>
void mergeSort(DataType theArray[], int first, int last, int arraySize)
{
	// Base case: Only one item in array partition
	if ((last-first) == 0)
		return;

	// Find the midpoint of the array
	int midPoint = (first + last) / 2;

	// Mergesort the first half of the array
	mergeSort(theArray, first, midPoint, arraySize);

	// Mergesort the second half of the array
	mergeSort(theArray, midPoint + 1, last, arraySize);

	// Merge the two lists together
	merge(theArray, first, midPoint, last, arraySize);

}

template<typename DataType>
void merge(DataType theArray[],
	int first, int mid, int last, int arraySize)
	// ---------------------------------------------------------
	// Merges two sorted array segments theArray[first..mid] and 
	// theArray[mid+1..last] into one sorted array.
	// Precondition: first <= mid <= last. The subarrays 
	// theArray[first..mid] and theArray[mid+1..last] are each 
	// sorted in increasing order.
	// Postcondition: theArray[first..last] is sorted.
	// Implementation note: This function merges the two
	// subarrays into a temporary array and copies the result
	// into the original array theArray.
	// ---------------------------------------------------------
{
	DataType *tempArray = new DataType[arraySize];    // temporary array

									 // initialize the local indexes to indicate the subarrays
	int first1 = first;       // beginning of first subarray
	int last1 = mid;         // end of first subarray
	int first2 = mid + 1;     // beginning of second subarray
	int last2 = last;        // end of second subarray

							 // while both subarrays are not empty, copy the
							 // smaller item into the temporary array
	int index = first1;    // next available location in 
						   // tempArray
	for (; (first1 <= last1) && (first2 <= last2); ++index)
	{  // Invariant: tempArray[first1..index-1] is in order
		if (theArray[first1] < theArray[first2])
		{
			tempArray[index] = theArray[first1];
			++first1;
		}
		else
		{
			tempArray[index] = theArray[first2];
			++first2;
		}  // end if
	}  // end for

	   // finish off the nonempty subarray

	   // finish off the first subarray, if necessary
	for (; first1 <= last1; ++first1, ++index)
		// Invariant: tempArray[first1..index-1] is in order
		tempArray[index] = theArray[first1];

	// finish off the second subarray, if necessary
	for (; first2 <= last2; ++first2, ++index)
		// Invariant: tempArray[first1..index-1] is in order
		tempArray[index] = theArray[first2];

	// copy the result back into the original array
	for (index = first; index <= last; ++index)
		theArray[index] = tempArray[index];

	delete tempArray;

}  // end merge


//******* QUICKSORT SECTION *********\\

template<typename DataType>
void choosePivot(DataType theArray[], int first, int last)
// ---------------------------------------------------------
// Chooses a pivot for quicksort’s partition algorithm and 
// swaps it with the first item in an array.
// Precondition: theArray[first..last] is an array; 
// first <= last.
// Postcondition: theArray[first] is the pivot.
// ---------------------------------------------------------
{
	// Use midpoint as pivot
	int pivIndex = (first + last) / 2;

	// Swap first index with pivot
	swap(theArray[first], theArray[pivIndex]);
}

template<typename DataType>
void partition(DataType theArray[],
	int first, int last, int& pivotIndex)
	// ---------------------------------------------------------
	// Partitions an array for quicksort.
	// Precondition: theArray[first..last] is an array; 
	// first <= last.
	// Postcondition: Partitions theArray[first..last] such 
	// that:
	//    S1 = theArray[first..pivotIndex-1] <  pivot
	//         theArray[pivotIndex]          == pivot
	//    S2 = theArray[pivotIndex+1..last]  >= pivot
	// Calls: choosePivot and swap.
	// ---------------------------------------------------------
{
	// place pivot in theArray[first]
	choosePivot(theArray, first, last);
	DataType pivot = theArray[first];     // copy pivot

										  // initially, everything but pivot is in unknown
	int lastS1 = first;           // index of last item in S1
	int firstUnknown = first + 1; // index of first item in 
								  // unknown

								  // move one item at a time until unknown region is empty
	for (; firstUnknown <= last; ++firstUnknown)
	{  // Invariant: theArray[first+1..lastS1] < pivot
	   //         theArray[lastS1+1..firstUnknown-1] >= pivot

	   // move item from unknown to proper region
		if (theArray[firstUnknown] < pivot)
		{  // item from unknown belongs in S1
			++lastS1;
			swap(theArray[firstUnknown], theArray[lastS1]);
		}  // end if

		   // else item from unknown belongs in S2
	}  // end for

	   // place pivot in proper position and mark its location
	swap(theArray[first], theArray[lastS1]);
	pivotIndex = lastS1;
}  // end partition

template<typename DataType>
void quickSort(DataType theArray[], int first, int last)
// ---------------------------------------------------------
// Sorts the items in an array into ascending order.
// Precondition: theArray[first..last] is an array.
// Postcondition: theArray[first..last] is sorted.
// Calls: partition.
// ---------------------------------------------------------
{
	int pivotIndex;

	if (first < last)
	{  // create the partition: S1, pivot, S2
		partition(theArray, first, last, pivotIndex);

		// sort regions S1 and S2
		quickSort(theArray, first, pivotIndex - 1);
		quickSort(theArray, pivotIndex + 1, last);
	}  // end if
}  // end quicksort

/*template<typename DataType>
void swap(DataType& x, DataType& y)
// ---------------------------------------------------
// Swaps two items.
// Precondition: x and y are the items to be swapped.
// Postcondition: Contents of actual locations that x 
// and y represent are swapped.
// ---------------------------------------------------
{
	DataType temp = x;
	x = y;
	y = temp;
}  // end swap*/