#include "Decoder.h"
#include <iostream>

int main()
{
	try
	{
		std::string line;
		while (std::getline(std::cin, line))
		{
			std::cout << HtmlDecode(line) << std::endl;
		}
	}
	catch (const std::exception& errorMessage)
	{
		std::cout << errorMessage.what() << std::endl;
		return 1;
	}
	return 0;
}