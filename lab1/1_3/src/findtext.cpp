#include <fstream>
#include <iostream>

bool CheckSubstringContainsInLine(const std::string& input, const std::string& substring)
{
	const std::string::size_type pos = input.find(substring);
	return pos != std::string::npos;
}

int FindString(const std::string& path, const std::string& desiredString)
{
	
	//подразбить на подфункции
	std::ifstream input(path);

	if (input.is_open())
	{

		std::string line;
		unsigned int lineNumber = 0;
		bool isStringFound = true;

		while (std::getline(input, line))
		{
			lineNumber++;
			if (CheckSubstringContainsInLine(line, desiredString))
			{
				isStringFound = false;
				std::cout << lineNumber << ": " << line << std::endl;
			}
		}

		if (!isStringFound)
		{
			std::cout << "Text not found" << std::endl;
			return 1;
		}
	}
	else
	{
		std::cerr << "Failed to open file" << std::endl;
		return 2;
	}

	input.close();
	return 0;
}