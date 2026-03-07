#include <iostream>
#include <string>
#include <ctime>

#include "PrimeGen.h"

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cerr << "Usage: " << argv[0] << " <upper bound>" << std::endl;
			return 1;
		}

		const uint64_t maxNumber = std::stoull(argv[1]);
		const time_t start = time(nullptr);
		const auto ans = GeneratePrimeNumbersSet(maxNumber);
		std::cout << ans.size() << " " << maxNumber << std::endl;
		const time_t stop = time(nullptr);
		const double seconds = difftime(stop, start);
		std::cout << "Time taken: " << seconds << " seconds" << std::endl;
	}
	catch (const std::exception& error)
	{
		std::cerr << "Ошибка: " << error.what() << std::endl;
		return 1;
	}

	return 0;
}