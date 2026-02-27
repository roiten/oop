#include "dict.h"
#include <iostream>

int main(const int argc, char* argv[])
{
	std::locale::global(std::locale("ru_RU.utf8"));
	try
	{
		const std::string fileName = GetFileName(argc, argv);

		Dictionary dictionary = LoadDictionary(fileName);
		RunDictionary(dictionary, fileName);
	}
	catch (const std::exception& errorMessage)
	{
		std::cerr << errorMessage.what() << std::endl;
		return 1;
	}

	return 0;
}