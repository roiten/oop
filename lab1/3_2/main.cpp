#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void readFile(std::ifstream& inputFile, double matrix[3][3])
{
    for (int rowIndex = 0; rowIndex < 3; rowIndex++)
    {
        std::string line;
        std::getline(inputFile, line);

        std::istringstream lineStream(line);

        for (int columnIndex = 0; columnIndex < 3; columnIndex++)
        {
            double value;
            lineStream >> value;
            matrix[rowIndex][columnIndex] = value;
        }
    }
}

double getMinorDeterminant(double matrix[3][3], int row, int column)
{
    double subMatrix[2][2];
    int subRowIndex = 0;

    for (int rowIndex = 0; rowIndex < 3; ++rowIndex)
    {
        if (rowIndex == row)
            continue;

        int subColumnIndex = 0;

        for (int columnIndex = 0; columnIndex < 3; ++columnIndex)
        {
            if (columnIndex == column)
                continue;

            subMatrix[subRowIndex][subColumnIndex] =
                matrix[rowIndex][columnIndex];

            subColumnIndex++;
        }

        subRowIndex++;
    }

    return subMatrix[0][0] * subMatrix[1][1]
         - subMatrix[0][1] * subMatrix[1][0];
}

double getDeterminant(double matrix[3][3])
{
    double determinant = 0.0;

    for (int columnIndex = 0; columnIndex < 3; ++columnIndex)
    {
        double cofactorSign =
            (columnIndex % 2 == 0) ? 1.0 : -1.0;

        determinant +=
            cofactorSign *
            matrix[0][columnIndex] *
            getMinorDeterminant(matrix, 0, columnIndex);
    }

    return determinant;
}

bool inverse(double matrix[3][3], double inverseMatrix[3][3])
{
    double determinant = getDeterminant(matrix);

    if (determinant == 0.0)
        return false;

    for (int rowIndex = 0; rowIndex < 3; ++rowIndex)
    {
        for (int columnIndex = 0; columnIndex < 3; ++columnIndex)
        {
            double cofactorSign =
                ((rowIndex + columnIndex) % 2 == 0) ? 1.0 : -1.0;

            inverseMatrix[columnIndex][rowIndex] =
                cofactorSign *
                getMinorDeterminant(matrix, rowIndex, columnIndex)
                / determinant;
        }
    }

    return true;
}

void drawMatrix(double matrix[3][3])
{
    for (int rowIndex = 0; rowIndex < 3; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < 3; columnIndex++)
        {
            std::cout << matrix[rowIndex][columnIndex] << " ";
        }
        std::cout << "\n";
    }

    std::cout << std::endl;
}

int main(int argumentCount, char* argumentValues[])
{
    try
    {
        double matrix[3][3];
        double inverseMatrix[3][3];

        if (argumentCount < 2)
        {
			std::cout << "Input first row: ";
        	std::cin >> matrix[0][0] >> matrix[0][1] >> matrix[0][2];
        	std::cout << "Input second row: ";
        	std::cin >> matrix[1][0] >> matrix[1][1] >> matrix[1][2];
        	std::cout << "Input third row: ";
        	std::cin >> matrix[2][0] >> matrix[2][1] >> matrix[2][2];
        }

    	else if (argumentCount == 2)
    	{
    		std::ifstream inputFile(argumentValues[1]);

    		if (!inputFile.is_open())
    		{
    			std::cerr << "Cannot open file\n";
    			return 1;
    		}

    		readFile(inputFile, matrix);
    	}

    	else
    	{
    		std::cout << "Use inverse -h" << std::endl;
    		return 1;
    	}

        if (!inverse(matrix, inverseMatrix))
        {
            std::cerr << "Matrix has no inverse\n";
            return 1;
        }

        std::cout.precision(2);
    	std::cout << "\n" << "Answer:" << std::endl;
        drawMatrix(inverseMatrix);

        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Error: " << exception.what() << '\n';
        return 1;
    }
}