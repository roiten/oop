#include <iostream>
#include "decoder.h"

int main()
{
	try
	{
		HtmlDecodeAndPrint(std::cin, std::cout);
	}
	catch (const std::exception& errorMessage)
	{
		std::cout << errorMessage.what() << '\n';
		return 1;
	}
	return 0;
}