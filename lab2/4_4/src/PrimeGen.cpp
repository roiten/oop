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
	std::vector isPrime(upperBound + 1, true);
	std::set<uint64_t> result;
	isPrime[0] = false;
	isPrime[1] = false;

	for (uint64_t i = 4; i < upperBound; i = i + 2)
	{
		isPrime[i] = false;
	}
	//убыстрить в 5 раза моно - подумать
	for (uint64_t i = 3; i * i <= upperBound; i = i + 2)
	{
		if (isPrime[i])
		{
			for (uint64_t j = i * i; j <= upperBound; j += i)
			{
				isPrime[j] = false;
			}
		}
	}
	for (uint64_t i = 2; i <= upperBound; i++)
	{
		if (isPrime[i])
			result.insert(i);
	}
	return result;
}