#include "Calculator.h"
#include "Controller.h"
#include <iostream>

int main()
{
	Calculator calculator;
	Controller controller(calculator, std::cin, std::cout);

	while (!std::cin.eof())
	{
		controller.HandleCommand();
	}

	return 0;
}