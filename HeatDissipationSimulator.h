#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

// Function to create a 2D float matrix with given dimensions
float **createMatrix(int X_dimension, int Y_dimension)
{
    if (X_dimension <= 0 || Y_dimension <= 0)
    {
        return nullptr; // Return nullptr for invalid dimensions
    }

    float **matrix = new float *[Y_dimension]; // Allocate memory for rows
    for (int i = 0; i < Y_dimension; i++)
    {
        matrix[i] = new float[X_dimension]; // Allocate memory for columns
    }

    // Initialize matrix elements to 0
    for (int i = 0; i < Y_dimension; i++)
    {
        for (int j = 0; j < X_dimension; j++)
        {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

// Function to delete a 2D float matrix and free memory
int deleteMatrix(float **matrix, int X_dimension, int Y_dimension)
{
    for (int i = 0; i < Y_dimension; i++)
    {
        delete[] matrix[i]; // Free memory for each row
    }
    delete[] matrix; // Free memory for the array of rows

    return 0;
}

// Overloaded function to delete a 2D uint8_t matrix and free memory
int deleteMatrix(uint8_t **matrix, int X_dimension, int Y_dimension)
{
    for (int i = 0; i < Y_dimension; i++)
    {
        delete[] matrix[i]; // Free memory for each row
    }
    delete[] matrix; // Free memory for the array of rows

    return 0;
}

// Function to print the 2D float matrix with indices
int printMatrix(float **matrix, int X_dimension, int Y_dimension)
{
    if (X_dimension <= 0 || Y_dimension <= 0)
    {
        return -1; // Return error for invalid dimensions
    }

    // Print column indices
    cout << "    ";
    for (int i = 0; i < X_dimension; i++)
    {
        cout << setw(10) << i << " ";
    }
    cout << endl;

    // Print row separator
    cout << "   ";
    for (int i = 0; i < X_dimension; i++)
    {
        cout << "___________";
    }
    cout << endl;

    // Print matrix with row indices
    for (int i = 0; i < Y_dimension; i++)
    {
        cout << i << " | ";
        for (int j = 0; j < X_dimension; j++)
        {
            cout << setw(10) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Function to set boundary values in the matrix
int SetBoundaryValues(float **matrix, int X_dimension, int Y_dimension, float top, float bottom, float left, float right)
{
    if (X_dimension <= 0 || Y_dimension <= 0)
    {
        return -1; // Return error for invalid dimensions
    }

    if (top <= 0 || bottom <= 0 || left <= 0 || right <= 0)
    {
        return -2; // Return error for invalid boundary values
    }

    // Set top and bottom boundaries
    for (int i = 0; i < X_dimension; i++)
    {
        matrix[0][i] = top;
        matrix[Y_dimension - 1][i] = bottom;
    }

    // Set left and right boundaries
    for (int i = 1; i < Y_dimension - 1; i++)
    {
        matrix[i][0] = left;
        matrix[i][X_dimension - 1] = right;
    }
    return 0;
}

// Function to set a specific temperature in the matrix at given coordinates
int stimulateGrid(float **matrix, int X_dimension, int Y_dimension, int X_coordinate, int Y_coordinate, float temp)
{
    if (X_dimension <= 0 || Y_dimension <= 0)
    {
        return -1; // Return error for invalid dimensions
    }

    if (temp <= 0)
    {
        return -2; // Return error for invalid temperature
    }

    if (X_coordinate <= 0 || X_coordinate >= X_dimension - 1 || Y_coordinate < 0 || Y_coordinate >= Y_dimension - 1)
    {
        return -3; // Return error for invalid coordinates
    }

    matrix[Y_coordinate][X_coordinate] = temp;
    return 0;
}

// Function to simulate thermal dissipation in the matrix until stability is reached
int SimulateThermalDissipation(float **matrix, int X_dimension, int Y_dimension, float threshold)
{
    if (X_dimension <= 0 || Y_dimension <= 0)
    {
        return -1; // Return error for invalid dimensions
    }
    if (threshold <= 0)
    {
        return -2; // Return error for invalid threshold
    }

    bool isStable = false;

    time_t start, end;

    time(&start);

    // Create a dynamic 2D array for storing the previous temperature values
    float **previousMatrix = createMatrix(X_dimension, Y_dimension);

    while (!isStable)
    {
        isStable = true; // Assume stability until proven otherwise

        // Iterate over the interior elements of the matrix
        for (int X = 1; X < X_dimension - 1; X++)
        {
            for (int Y = 1; Y < Y_dimension - 1; Y++)
            {
                // Calculate the new temperature based on surrounding values
                double newTemperature = (matrix[Y - 1][X] + matrix[Y + 1][X] + matrix[Y][X - 1] + matrix[Y][X + 1]) / 4;

                // Check for stability
                if (abs(newTemperature - matrix[Y][X]) >= threshold)
                {
                    isStable = false; // The temperature has not stabilized
                }

                // Update the matrix with the new temperature
                previousMatrix[Y][X] = matrix[Y][X];
                matrix[Y][X] = newTemperature;
            }
        }
    }

    time(&end);

    deleteMatrix(previousMatrix, X_dimension, Y_dimension);

    return end - start; // Return the time taken for simulation
}

// Function to convert a 2D float matrix to an 8-bit matrix for visualization
uint8_t **MatrixFloatTo8bit(float **matrix, int X_dimension, int Y_dimension)
{
    float MaxTemp = -1;
    float MinTemp = 999999999;

    if (X_dimension <= 0 || Y_dimension <= 0)
    {
        return nullptr; // Return nullptr for invalid dimensions
    }

    // Find the maximum and minimum temperatures in the matrix
    for (int i = 0; i < Y_dimension; i++)
    {
        for (int j = 0; j < X_dimension; j++)
        {
            if (matrix[i][j] > MaxTemp)
            {
                MaxTemp = matrix[i][j];
            }
            if (matrix[i][j] < MinTemp)
            {
                MinTemp = matrix[i][j];
            }
        }
    }

    // Create an 8-bit matrix and scale temperatures accordingly
    uint8_t **copiedMatrix = new uint8_t *[Y_dimension];
    for (int i = 0; i < Y_dimension; i++)
    {
        copiedMatrix[i] = new uint8_t[X_dimension];
        for (int j = 0; j < X_dimension; j++)
        {
            copiedMatrix[i][j] = static_cast<uint8_t>((matrix[i][j] - MinTemp) / (MaxTemp - MinTemp) * 255);
        }
    }
    return copiedMatrix;
}