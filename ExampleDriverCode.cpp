#include <bits/stdc++.h>
#include "Bitmap_Helper.h"
#include "HeatDissipationSimulator.h"
using namespace std;

// The functions are written to be idiot-proof, but main() still makes preliminary input checks
int main()
{
    // Variables to initialize the matrix
    float **matrix = nullptr;
    int X_dimension = -1, Y_dimension = -1;

    // Variables to set boundary conditions
    string BoundaryNames[4] = {"top", "bottom", "left", "right"};
    float BoundaryTemperatures[4] = {-1, -1, -1, -1};

    // Variables to set specific stimuli temperatures
    float temp = -1;
    int X_coordinate = -1, Y_coordinate = -1;

    // Variables to set simulation parameters
    float threshold = -1;
    int timeTaken;
    uint8_t **EightBitMatrix = nullptr;
    string filename = "HeatDissipationSimulation";
    string input;

    cout << "Hello User" << endl;
    cout << "This program simulates heat distribution in 2D metallic objects" << endl;
    cout << "All values must be in Kelvins!!!" << endl;
    cout << endl;

    bool restart = true;
    while (restart)
    {
        switch (CallMainMenu())
        {
        case '1': // Create a 2D surface
            while (X_dimension <= 0 || Y_dimension <= 0)
            {
                cout << "Enter the width (X) of the 2D surface: ";
                cin >> X_dimension;
                cout << "Enter the height (Y) of the 2D surface: ";
                cin >> Y_dimension;
                if (X_dimension <= 0 || Y_dimension <= 0)
                {
                    cout << "Invalid dimensions!" << endl;
                    cout << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            if (createMatrix(matrix, X_dimension, Y_dimension) == 0)
            {
                cout << "The 2D surface matrix has been created!!!" << endl;
                cout << endl;
            }
            else
            {
                cout << "Failed to create the 2D surface matrix!!!" << endl;
                cout << endl;
                return -1;
            }
            break;

        case '2': // Set boundary values
            if (matrix == nullptr)
            {
                cout << "The matrix has not been created yet!!!" << endl;
                cout << endl;
                break;
            }
            cout << "Now the program will prompt you to set the boundary temperatures" << endl;
            for (int i = 0; i < 4; i++)
            {
                while (BoundaryTemperatures[i] <= 0)
                {
                    cout << "Enter the temperature of the " << BoundaryNames[i] << " boundary: ";
                    cin >> BoundaryTemperatures[i];
                    if (BoundaryTemperatures[i] <= 0)
                    {
                        cout << "Invalid temperature!" << endl;
                        cout << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }
            }
            SetBoundaryValues(matrix, X_dimension, Y_dimension, BoundaryTemperatures[0], BoundaryTemperatures[1], BoundaryTemperatures[2], BoundaryTemperatures[3]);
            cout << "The boundary temperatures have been set!!!" << endl;
            cout << endl;
            break;

        case '3': // Set stimulus point
            if (matrix == nullptr)
            {
                cout << "The matrix has not been created yet!!!" << endl;
                cout << endl;
                break;
            }
            cout << "Now the programm will prompt the coordinates of the stimulation point" << endl;
            while (X_coordinate <= 0 || X_coordinate >= X_dimension - 1 || Y_coordinate <= 0 || Y_coordinate >= Y_dimension - 1)
            {
                cout << "Enter the X coordinate of the stimulation point: ";
                cin >> X_coordinate;
                cout << "Enter the Y coordinate of the stimulation point: ";
                cin >> Y_coordinate;
                if (X_coordinate <= 0 || X_coordinate >= X_dimension - 1 || Y_coordinate <= 0 || Y_coordinate >= Y_dimension - 1)
                {
                    cout << "Invalid cordinates!" << endl;
                    cout << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            while (temp <= 0)
            {
                cout << "Enter the set temperature of the stimulation point: ";
                cin >> temp;
                if (temp <= 0)
                {
                    cout << "Invalid temperature!" << endl;
                    cout << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            stimulateGrid(matrix, X_dimension, Y_dimension, X_coordinate, Y_coordinate, temp);
            cout << endl;
            break;

        case '4': // Simulate thermal dissipation
            if (matrix == nullptr)
            {
                cout << "The matrix has not been created yet!!!" << endl;
                cout << endl;
                break;
            }
            cout << "Now the program will prompt the simulation threshold to simulate the thermal dissipation" << endl;
            while (threshold <= 0)
            {
                cout << "Enter the simulation threshold: ";
                cin >> threshold;
                if (threshold <= 0)
                {
                    cout << "Invalid threshold!" << endl;
                    cout << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            cout << endl;
            cout << "Simulation in progress..." << endl;
            cout << "It may take some time. Grab yourself a coffee" << endl;

            timeTaken = SimulateThermalDissipation(matrix, X_dimension, Y_dimension, threshold);
            cout << "Simulation successful !!!" << endl;
            cout << "The simulation took " << timeTaken << " seconds" << endl;
            cout << endl;

            // Create a Bitmap image
            cout << "Enter the name for the bitmap file without extension (use ! to use the default name '" << filename << ".bmp'): ";
            cin >> input;
            if (input != "!")
            {
                filename = input;
            }
            createMatrix(EightBitMatrix, X_dimension, Y_dimension);
            MatrixFloatTo8bit(matrix, EightBitMatrix, X_dimension, Y_dimension);
            writeBitmap((filename + ".bmp").c_str(), EightBitMatrix, X_dimension, Y_dimension);
            cout << "The bitmap file " << filename << ".bmp has been created!!!" << endl;
            cout << endl;

            // Delete allocated memory
            deleteMatrix(EightBitMatrix, X_dimension, Y_dimension);
            break;

        case '5':
            cout << "Keep in mind that the matrix can be too large for the screen!!!" << endl;
            cout << "Would you like to see the final matrix? (y/n): ";
            char show;
            cin >> show;
            if (show == 'y')
            {
                if (matrix != nullptr)
                {
                    printMatrix(matrix, X_dimension, Y_dimension);
                }
                else
                {
                    cout << "The matrix is empty!" << endl;
                    cout << endl;
                }
            }
            else if (show == 'n')
            {
                break;
            }
            else
            {
                cout << "Invalid input!" << endl;
                cout << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << endl;
            break;

        case '6':
            cout << "Goodbye User" << endl;
            deleteMatrix(matrix, X_dimension, Y_dimension);
            system("pause");
            return 0;
            break;

        default:
            cout << "Invalid input" << endl;
            cout << "Restarting ..." << endl;
            cout << endl;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
}
