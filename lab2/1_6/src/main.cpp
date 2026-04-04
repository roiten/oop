#include "TransformVector.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

constexpr size_t PRECISION = 3;

void PrintSortedNumbers(const std::vector<double>& numbers);
void ReadNumbers(std::vector<double>& numbers);

int main()
{
	try
	{
		std::vector<double> numbers;
		ReadNumbers(numbers);
		std::ranges::sort(numbers);
		ProcessNumbers(numbers);
		PrintSortedNumbers(numbers);
	}
	catch (const std::exception& errorMessage)
	{
		std::cout << errorMessage.what() << std::endl;
		return 1;
	}
	return 0;
}

void ReadNumbers(std::vector<double>& numbers)
{
	std::string line;

	while (std::getline(std::cin, line))
	{
		if (line.empty())
			break;

		std::istringstream lineStream(line);
		double number;

		while (lineStream >> number)
		{
			numbers.push_back(number);
		}

		if (!lineStream.eof())
		{
			throw std::runtime_error("Invalid data");
		}
	}
}

void SetupPrecision()
{
	std::cout << std::fixed;
	std::cout << std::setprecision(PRECISION);
}

void PrintSortedNumbers(const std::vector<double>& numbers)
{
	SetupPrecision();

	for (const double number : numbers)
	{
		std::cout << number << " ";
	}
	std::cout << std::endl;
}