#include "vector.h"
#include <iostream>

int main()
{
	try
	{
		std::vector<double> numbers;
		RunApp(numbers);
	}
	catch (const std::exception& errorMessage)
	{
		std::cout << errorMessage.what() << std::endl;
		return 1;
	}
	return 0;
}