#include "Simulation/Simulation.h"
#include <iostream>
#include <string>

struct Arguments
{
	int cash;
	int days;
};

Arguments ParseArgs(int argc, char const* argv[])
{
	Arguments args;
	if (argc != 3)
	{
		throw std::invalid_argument("Аргументы введены неправильно: \n"
									"1. Имя программы \n"
									"2. Количество денег в системе \n"
									"3. Количество дней симуляции\n");
	}
	args.cash = std::stoi(argv[1]);
	args.days = std::stoi(argv[2]);
	return args;
}

int main(int argc, char const* argv[])
{
	Arguments args;

	try
	{
		auto args = ParseArgs(argc, argv);
		Simulation simulation(args.cash, args.days);
		simulation.RunSimulation();
	}
	catch (const std::exception& error)
	{
		std::cerr << "Error: " << error.what() << std::endl;
	}

	return 0;
}