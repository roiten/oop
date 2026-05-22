#include "FindMax.h"
#include <iostream>
#include <string>
#include <vector>

#include "FindMax.h"
#include "FindMaxEx.h"
#include "Sportsmen.h"
#include <iostream>
#include <vector>

int main()
{
	std::vector<Sportsmen> Sportsmens = {
		Sportsmen("Иван Иванов", 180, 75),
		Sportsmen("Петр Петров", 195, 82),
		Sportsmen("Сидор Сидоров", 175, 92),
		Sportsmen("Алексей Алексеев", 185, 70),
		Sportsmen("Дмитрий Дмитриев", 190, 88)
	};
    
	std::cout << "Список спортсменов:\n";
	for (const auto& a : Sportsmens)
	{
		std::cout << "  " << a << std::endl;
	}
	std::cout << std::endl;
    
	Sportsmen tallest;
	if (FindMaxEx(Sportsmens, tallest, CompareByHeight()))
	{
		std::cout << "Спортсмен с максимальным ростом:\n  " << tallest << std::endl;
	}
    
	std::cout << std::endl;
    
	Sportsmen heaviest;
	if (FindMaxEx(Sportsmens, heaviest, CompareByWeight()))
	{
		std::cout << "Спортсмен с максимальным весом:\n  " << heaviest << std::endl;
	}
    
	std::cout << std::endl;
	return 0;
}