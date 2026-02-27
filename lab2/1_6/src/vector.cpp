#include "vector.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

constexpr size_t PRECISION = 3;
constexpr size_t MINIMAL_NUMBERS_SUM_COUNT = 3;

void ReadNumbers(std::vector<double>& numbers)
{
	std::string line;

	while (std::getline(std::cin, line))
	{
		if (line.empty()) break;

		std::istringstream lineStream(line);
		double number;

		while (lineStream >> number)
		{
			numbers.push_back(number);
		}

		if (!lineStream.eof())
		{
			throw std::exception("Invalid data");
		}
	}
}

void ProcessNumbers(std::vector<double>& numbers)
{
	std::vector <double> minimalNumbers(MINIMAL_NUMBERS_SUM_COUNT);
	std::partial_sort_copy(numbers.begin(), numbers.end(), minimalNumbers.begin(), minimalNumbers.end());
	const double sum = std::accumulate(minimalNumbers.begin(), minimalNumbers.end(), 0.0);

	for (double& number : numbers)
	{
		number += sum;
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

void RunApp(std::vector<double>& numbers)
{
	ReadNumbers(numbers);
	std::ranges::sort(numbers);
	ProcessNumbers(numbers);
	PrintSortedNumbers(numbers);
}