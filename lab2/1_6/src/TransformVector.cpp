#include "TransformVector.h"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <vector>

constexpr size_t MINIMAL_NUMBERS_COUNT = 3;

void ProcessNumbers(std::vector<double>& numbers)
{
	std::vector<double> minimalNumbers(MINIMAL_NUMBERS_COUNT);
	std::partial_sort_copy(numbers.begin(), numbers.end(), minimalNumbers.begin(), minimalNumbers.end());
	const double sum = std::accumulate(minimalNumbers.begin(), minimalNumbers.end(), 0.0);

	std::ranges::transform(numbers, numbers.begin(), [sum](const double n) { return n + sum; });
}