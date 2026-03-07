#include <climits>
#include <cstdint>
#include <set>
#include <stdexcept>
#include <vector>

std::set<uint64_t> GeneratePrimeNumbersSet(const uint64_t upperBound)
{
	if (upperBound < 2)
	{
		return {};
	}

	if (upperBound > INT_MAX - 1)
	{
		throw std::overflow_error("upperBound слишком большой");
	}

	std::vector<bool> isPrime(upperBound + 1, true);
	std::set<uint64_t> result;

	isPrime[0] = false;
	isPrime[1] = false;

	for (uint64_t i = 2; i <= upperBound; ++i)
	{
		if (isPrime[i])
		{
			result.insert(i);

			if (i <= upperBound / i)
			{
				for (uint64_t j = i * i; j <= upperBound; j += i)
				{
					isPrime[j] = false;
				}
			}
		}
	}

	return result;
}