#include <bits/stdc++.h>
#include "Bitmap_Helper.h"
#include "HeatDissipationSimulator.h"
using namespace std;

// The functions are written to be idiot-proof, but main() still makes preliminary input checks
int main()
{
    cout << "Hello User" << endl;
    cout << "This program simulates heat distribution in 2D metallic objects" << endl;
    cout << "All values must be in Kelvins!!!" << endl;
    cout << endl;
    cout << "Start the program? (y/n): ";
    char start;
    cin >> start;
    if (start == 'n')
    {
        cout << "Goodbye User" << endl;
        return 0;
    }
    else if (start == 'y')
    {
        // Create a 2D surface
        int X_dimension = -1, Y_dimension = -1;
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
        float **matrix = createMatrix(X_dimension, Y_dimension);
        cout << "The 2D surface matrix has been created!!!" << endl;
        cout << endl;

        // Set boundary values
        cout << "Now the program will prompt you to set the boundary temperatures" << endl;

        string BoundaryNames[4] = {"top", "bottom", "left", "right"};
        float BoundaryTemperatures[4] = {-1, -1, -1, -1};

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

        // Stimulate
        cout << "Now the programm will prompt the coordinates of the stimulation point" << endl;
        float temp = -1;
        int X_coordinate = -1, Y_coordinate = -1;
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

        // Initial Bitmap
        string filename = "Initial";
        string input;
        cout << "Enter the name for the bitmap file without extension (use ! to use the default name '" << filename << "'): ";
        cin >> input;
        if (input != "!")
        {
            filename = input;
        }
        uint8_t **EightBitMatrix = MatrixFloatTo8bit(matrix, X_dimension, Y_dimension);
        writeBitmap((filename + ".bmp").c_str(), EightBitMatrix, X_dimension, Y_dimension);
        cout << "The bitmap file " << filename << " has been created!!!" << endl;
        cout << endl;
        deleteMatrix(EightBitMatrix, X_dimension, Y_dimension);

        // Simulate thermal dissipation
        cout << "Now the program will prompt the simulation threshold to simulate the thermal dissipation" << endl;
        float threshold = -1;
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
        int timeTaken = SimulateThermalDissipation(matrix, X_dimension, Y_dimension, threshold);
        cout << "Simulation successful !!!" << endl;
        cout << "The simulation took " << timeTaken << " seconds" << endl;
        cout << endl;

        // Final Bitmap
        filename = "Final";
        cout << "Enter the name for the bitmap file without extension (use ! to use the default name '" << filename << "'): ";
        cin >> input;
        if (input != "!")
        {
            filename = input;
        }
        EightBitMatrix = MatrixFloatTo8bit(matrix, X_dimension, Y_dimension);
        writeBitmap((filename + ".bmp").c_str(), EightBitMatrix, X_dimension, Y_dimension);
        cout << "The bitmap file " << filename << " has been created!!!" << endl;
        cout << endl;

        // Delete allocated memory
        deleteMatrix(EightBitMatrix, X_dimension, Y_dimension);
        deleteMatrix(matrix, X_dimension, Y_dimension);

        cout << "Program finished" << endl;
        cout << "Restarting ..." << endl;
        cout << endl;
        main();
    }
    else
    {
        cout << "Invalid input" << endl;
        cout << endl;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        main();
    }
}
