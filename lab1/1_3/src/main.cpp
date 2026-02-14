#include <fstream>
#include <iostream>
#include <string>

bool checkSubstringContainsInLine(const std::string& input, const std::string& substring)
{
	const std::string::size_type pos = input.find(substring);
	return pos != std::string::npos;
}

int findString(std::ifstream& input, const std::string& desiredString)
{
	std::string line;
	unsigned int lineNumber = 0;
	bool isStringFound = false;

	while (std::getline(input, line))
	{
		lineNumber++;
		if (checkSubstringContainsInLine(line, desiredString))
		{
			isStringFound = true;
			std::cout << lineNumber << std::endl;
		}
	}

	if (!isStringFound)
	{
		std::cout << "Text not found" << std::endl;
		return 1;
	}

	return 0;
}

int main(const int argc, char* argv[])
{
	if (argc != 3 || std::string(argv[2]).empty())
	{
		std::cerr << "Usage: findtext <file> <text>" << std::endl;
		return 3;
	}

	std::ifstream input(argv[1]);
	if (!input.is_open())
	{
		std::cerr << "Failed to open file: " << argv[1] << std::endl;
		return 2;
	}

	return findString(input, argv[2]);
}
