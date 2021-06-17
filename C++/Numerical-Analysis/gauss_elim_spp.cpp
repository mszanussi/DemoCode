#include <iostream>
#include <iomanip>
#include <cstddef>

using namespace std;


// ROW SWAP:
//  Swaps two rows on a matrix, assuming
//  that the rows are represented by a pointer
//  pointing to an array.
void SwapRows(float** matrix, int row1, int row2);

// PRINT MATRIX:
//  Iterates through a matrix and prints
//  each item assuming rows come before
//  columns.
void PrintMatrix(float** matrix, int rows, int columns);

// DESTROY MATRIX:
//  Deallocates information stored in a two
//  dimensional array representing a matrix
void DestroyMatrix(float**& matrix, int rows, int columns);

// SCALE ROWS:
//  Scales each row such that the maximum element has
//  an absolute value of 1. Returns false if an entire
//  row is zero.
bool ScaleRows(float** matrix, int rows, int columns, float* imageVector);

// GUASSIAN ELIMINATION:
//  Performs Gaussian elimination on a matrix using
//  scaled partial pivoting.
void GaussianElimination(float** matrix, int matrixSize, float* imageVector);

// PARTIAL PIVOT
//  Swaps rows in the matrix to ensure that there is
//  no accidental zero or close-to-zero division during
//  Gaussian elimination.
void PartialPivot(float** matrix, int matrixSize, int elimStep, float* imageVector);

// BACK SUBSTITUTION:
//  Propagates back up through the upper-triangular matrix
//  to set the imageVector to the pre-image of the system
void BackSubstitution(float** matrix, int matrixSize, float* imageVector);

// ABSOLUTE FLOAT:
float absf(float number);

int main()
{
    // Matrix to store user data
    float** matrix;
    float* imageVector;

    // Variables to store any user input
    int ROWS, COLUMNS;

    // Prompt user for matrix size
    cout << "How many rows and columns: ";
    cin >> ROWS >> COLUMNS;

    if( ROWS != COLUMNS )
    {
        cerr << "ERROR: Matrix not invertible. This system only solves square matrices." << endl;
        return -1;
    }

    // Generate ROW many columns
    matrix = new float*[ROWS];
    imageVector = new float[ROWS];

    // Generates COLUMN many items for each row
    for(int i = 0; i < ROWS; ++i){
        matrix[i] = new float[COLUMNS];
    }

    // Prompt user for matrix entries
    cout << "Input the matrix: " << endl;

    // For each item in the matrix
    for(int i = 0; i < ROWS; ++i){
        for(int j = 0; j < COLUMNS; ++j)
            cin >> matrix[i][j];
    }

    cout << "Input the image vector:" << endl;

    for(int i = 0; i < ROWS; ++i)
    {
        cin >> imageVector[i];
    }

    // Scale matrix
    if(!ScaleRows(matrix, ROWS, COLUMNS, imageVector))
    {
        cerr << "ERROR: Matrix not invertible. One row is entirely zero." << endl;
        return -1;
    }

    cout << endl << endl << "SCALED MATRIX:" << endl;
    PrintMatrix(matrix,ROWS,COLUMNS);

    cout << endl << endl << "ELIMINATED MATRIX:" << endl;
    GaussianElimination(matrix,ROWS, imageVector);
    PrintMatrix(matrix, ROWS,COLUMNS);

    cout << endl << endl << "ELIMINATED IMAGE VECTOR:" << endl;

    for(int i = 0; i < ROWS; ++i)
        cout << imageVector[i] << endl;

    // Take determinant via product of diagonal of matrix
    float determinant = 1;
    for(int i = 0; i < ROWS; ++i)
        determinant *= matrix[i][i];

    // If matrix is invertible, it has a unique solution
    if(determinant != 0)
    {
        cout << endl << endl << "SOLUTION VECTOR:" << endl;
        BackSubstitution(matrix,ROWS,imageVector);

        for(int i = 0; i < ROWS; ++i)
            cout << imageVector[i] << endl;
    }

    else
    {
        cout << endl << endl << "The matrix does not have a unique solution." << endl;
    }

    DestroyMatrix(matrix, ROWS, COLUMNS);

    return 0;
}

// ROW SWAP:
//  Swaps two rows on a matrix, assuming
//  that the rows are represented by a pointer
//  pointing to an array.
void SwapRows(float** matrix, int row1, int row2)
{
    // Create temp for swapping
    float* tempPtr;

    // Use tempPtr to swap pointer values in matrix
    // Don't swap if rows are the same
    if(row1 != row2)
    {
        tempPtr = matrix[row1];
        matrix[row1] = matrix[row2];
        matrix[row2] = tempPtr;
    }

    return;
}

// PRINT MATRIX:
//  Iterates through a matrix and prints
//  each item assuming rows come before
//  columns.
void PrintMatrix(float** matrix, int rows, int columns)
{
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; ++j){
            // If this is not the last item in the row
            if(j < columns - 1){
                // Make space for the next item
                cout << left << setw(13) << matrix[i][j] << " ";
            }

            // Last item in row
            else {
                // Move to next row
                cout << left << setw(13) << matrix[i][j] << endl;
            }
        }
    }

    return;
}

// DESTROY MATRIX:
//  Deallocates information stored in a two
//  dimensional array representing a matrix
void DestroyMatrix(float**& matrix, int rows, int columns)
{
    // Deallocate all information pointed at by
    // each pointer
    for(int i = 0; i < rows; ++i){
        delete[] matrix[i];
    }

    // Delete the pointer array
    delete[] matrix;
    matrix = NULL;

    return;
}

// SCALE ROWS:
//  Scales each row such that the maximum element has
//  an absolute value of 1. Returns false if
bool ScaleRows(float** matrix, int rows, int cols, float* imageVector)
{
    double rowMaxAbs = 0;

    for(int i = 0; i < rows; ++i)
    {
        // Assume first item is zero
        rowMaxAbs = double(matrix[i][0]);

        // Find maximum absolute value
        for(int j = 0; j < cols; ++j)
        {
            if(double(absf(matrix[i][j])) > rowMaxAbs)
                rowMaxAbs = double(absf(matrix[i][j]));
        }

        // If an entire row is zero
        if(rowMaxAbs == 0)
            return false;

        // Scale the row
        for(int j = 0; j < cols; ++j)
        {
            matrix[i][j] = double(matrix[i][j])/rowMaxAbs;
        }

        imageVector[i] = double(imageVector[i])/rowMaxAbs;
    }


    return true;
}

float absf(float number)
{
    return (number < 0) ? (-1 * number) : (number);
}

// GAUSSIAN ELIMINATION:
//  Performs Gaussian elimination on a matrix using
//  scaled partial pivoting. Only reduces square matrices.
void GaussianElimination(float** matrix, int matrixSize, float* imageVector)
{
    // Multiplier for eliminating rows based on pivot
    double multiplier;

    // For every row in the matrix
    for(int i = 0; i < matrixSize-1; ++i)
    {
        //Pivot the column that must be eliminated
        PartialPivot(matrix, matrixSize, i, imageVector);
        //PrintMatrix(matrix, matrixSize, matrixSize);

        // For every row under the current row
        for(int j = i + 1; j < matrixSize; ++j)
        {
            // Generate elimination multiplier so the item under
            // matrix[i][i] is reduced to zero
            multiplier = double(matrix[j][i])/double(matrix[i][i]);

            // For every item in the row
            for(int k = i; k < matrixSize; ++k)
            {
                // Row subtract the current row from each row under it
                matrix[j][k] = double(matrix[j][k]) - (multiplier * double(matrix[i][k]));
            }

            imageVector[j] = double(imageVector[j]) - (multiplier * double(imageVector[i]));
        }
    }

}

// PARTIAL PIVOT
//  Swaps rows in the matrix to ensure that there is
//  no accidental zero or close-to-zero division during
//  Gaussian elimination. Only used in Gaussian Elimination
void PartialPivot(float** matrix, int matrixSize, int elimStep, float* imageVector)
{
    int maxIndex = elimStep;
    for(int i = elimStep; i < matrixSize; ++i)
    {
        if(absf(matrix[i][elimStep]) > absf(matrix[maxIndex][elimStep]))
            maxIndex = i;
    }

    SwapRows(matrix, elimStep, maxIndex);

    // Swap items on image vector
    float temp = 0;
    temp = imageVector[maxIndex];
    imageVector[maxIndex] = imageVector[elimStep];
    imageVector[elimStep] = temp;

}

// BACK SUBSTITUTION:
//  Propagates back up through the upper-triangular matrix
//  to calculate the preimage vector and stores in in
//  the image vector
void BackSubstitution(float** matrix, int matrixSize, float* imageVector)
{
    float sum;

    //Find the first pre-image value
    imageVector[matrixSize-1] = (imageVector[matrixSize-1])/(matrix[matrixSize-1][matrixSize-1]);

    // For every row
    for(int i = matrixSize-2; i >= 0; --i)
    {
        sum = 0;
        // For every item in the row already solved for
        for(int j = i + 1; j < matrixSize; ++j)
            sum += (matrix[i][j])*imageVector[j];

        // Subtract sum from image and divide by coefficient
        imageVector[i] = (imageVector[i] - sum)/matrix[i][i];
    }

    return;
}
