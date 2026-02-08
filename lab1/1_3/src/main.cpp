#include "findtext.h"
#include <iostream>


// файлы называем КамелКейсом
// тесты пишем через sh
// функция НайтиСтроку называется неявно и слишком много на себя берёт

int main(const int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: findtext <file> <text>" << std::endl;
		return 3;
	}

	return FindString(argv[1], argv[2]);
}
