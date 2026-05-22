#include "sort2_tests.h"

#include <iostream>
#include <string>

int main()
{
	{
		std::cout << "Enter two doubles: ";
		double first, second;

		std::cin >> first >> second;
		Sort2(first, second);
		std::cout << first << "<" << second << std::endl;
	}

	{
		std::cout << "Enter two strings: ";
		std::string s1, s2;

		std::getline(std::cin, s1);
		std::getline(std::cin, s2);

		Sort2(s1, s2);
		std::cout << s1 << "<" << s2 << std::endl;
	}

	{
		std::cout << "Enter two c_strings: ";
		std::string s1, s2;

		std::getline(std::cin, s1);
		std::getline(std::cin, s2);

		const char* p1 = s1.c_str();
		const char* p2 = s2.c_str();

		Sort2(p1, p2);
		std::cout << p1 << "<" << p2 << std::endl;
	}

	{
		std::cout << "Enter three numbers: ";
		int first, second, third;

		std::cin >> first >> second >> third;
		Sort2(first, second);
		Sort2(first, third);

		std::cout << first << "<";

		Sort2(second, third);
		std::cout << second << "<" << third << std::endl;
	}
}