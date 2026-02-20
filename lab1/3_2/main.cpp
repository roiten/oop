#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void ReadFile(std::ifstream& inputFile, std::array<std::array<double, 3>, 3>& matrix)
{
	for (int rowIndex = 0; rowIndex < 3; rowIndex++)
	{
		std::string line;
		std::getline(inputFile, line);

		int delimCount = 0;
		for (const char c : line)
		{
			if (c == '\t' || c == ' ')
				delimCount++;
		}

		if (delimCount != 2)
		{
			throw std::exception("Invalid matrix format");
		}

		std::istringstream lineStream(line);

		for (int columnIndex = 0; columnIndex < 3; columnIndex++)
		{
			double value;
			lineStream >> value;
			matrix.at(rowIndex).at(columnIndex) = value;
		}
	}
}

double GetMinorDeterminant(const std::array<std::array<double, 3>, 3>& matrix, const int row, const int column)
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

			subMatrix[subRowIndex][subColumnIndex] = matrix[rowIndex][columnIndex];

			subColumnIndex++;
		}

		subRowIndex++;
	}

	return subMatrix[0][0] * subMatrix[1][1]
		- subMatrix[0][1] * subMatrix[1][0];
}

double GetDeterminant(const std::array<std::array<double, 3>, 3>& matrix)
{
	double determinant = 0.0;

	for (int columnIndex = 0; columnIndex < 3; ++columnIndex)
	{
		double cofSign = (columnIndex % 2 == 0) ? 1.0 : -1.0;

		determinant += cofSign * matrix[0][columnIndex] * GetMinorDeterminant(matrix, 0, columnIndex);
	}

	return determinant;
}

bool Inverse(const std::array<std::array<double, 3>, 3>& matrix, std::array<std::array<double, 3>, 3>& inverseMatrix)
{
	const double determinant = GetDeterminant(matrix);

	if (determinant == 0.0)
		return false;

	for (int rowIndex = 0; rowIndex < 3; ++rowIndex)
	{
		for (int columnIndex = 0; columnIndex < 3; ++columnIndex)
		{
			double cofSign = ((rowIndex + columnIndex) % 2 == 0) ? 1.0 : -1.0;

			inverseMatrix[columnIndex][rowIndex] = cofSign * GetMinorDeterminant(matrix, rowIndex, columnIndex)
				/ determinant;
		}
	}

	return true;
}

void DrawMatrix(const std::array<std::array<double, 3>, 3>& matrix)
{
	std::cout.precision(3);

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

// Coding conventions соблюдать
int main(int argc, char* argv[])
{
	try
	{
		std::array<std::array<double, 3>, 3> matrix{};
		std::array<std::array<double, 3>, 3> inverseMatrix{};

		if (argc < 2)
		{
			std::string line;

			for (int rowIndex = 0; rowIndex < 3; rowIndex++)
			{
				std::cout << "Input row " << (rowIndex + 1) << ": ";
				std::getline(std::cin, line);

				int delimCount = 0;
				for (char c : line)
				{
					if (c == '\t' || c == ' ')
						delimCount++;
				}

				if (delimCount != 2)
				{
					throw std::runtime_error("Invalid matrix format");
				}

				std::istringstream lineStream(line);
				lineStream >> matrix[rowIndex][0] >> matrix[rowIndex][1] >> matrix[rowIndex][2];
			}
		}

		else if (argc == 2)
		{

			if (std::string(argv[1]) == "-h")
			{
				std::cout << "Use this file to find inversed matrix" << std::endl;
				std::cout << "Using <./inverse> <matrix_file.txt>" << std::endl;
				return 0;
			}

			std::ifstream inputFile(argv[1]);

			if (!inputFile.is_open())
			{
				throw std::exception("Cannot open file");
			}

			ReadFile(inputFile, matrix);
		}

		else
		{
			std::cout << "Use inverse -h" << std::endl;
			return 1;
		}

		if (!Inverse(matrix, inverseMatrix))
		{
			std::cerr << "Non-invertible\n";
			return 0;
		}

		std::cout << "\n"
				  << "Answer:" << std::endl;

		DrawMatrix(inverseMatrix);

		return 0;
	}
	catch (const std::exception& exception)
	{
		std::cerr << "Error: " << exception.what() << '\n';
		return 1;
	}
}