#include "Car.h"
#include <iostream>
#include <sstream>
#include <string>

int main()
{
	Car car;
	std::string line;

	while (std::getline(std::cin, line))
	{
		if (line.empty())
			continue;

		std::istringstream iss(line);
		std::string command;
		iss >> command;

		if (command == "Info")
		{
			car.Info();
		}
		else if (command == "EngineOn")
		{
			try
			{
				car.TurnOnEngine();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else if (command == "EngineOff")
		{
			try
			{
				car.TurnOffEngine();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else if (command == "SetGear")
		{
			std::string arg;
			if (!(iss >> arg))
			{
				std::cout << "Invalid command argument" << std::endl;
				continue;
			}
			try
			{
				const int gear = std::stoi(arg);
				car.SetGear(gear);
			}
			catch (const std::invalid_argument&)
			{
				std::cout << "Invalid command argument" << std::endl;
			}
			catch (const std::out_of_range&)
			{
				std::cout << "Invalid command argument" << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else if (command == "SetSpeed")
		{
			std::string arg;
			if (!(iss >> arg))
			{
				std::cout << "Invalid command argument" << std::endl;
				continue;
			}
			try
			{
				const int speed = std::stoi(arg);
				car.SetSpeed(speed);
			}
			catch (const std::invalid_argument&)
			{
				std::cout << "Invalid command argument" << std::endl;
			}
			catch (const std::out_of_range&)
			{
				std::cout << "Invalid command argument" << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else
		{
			std::cout << "Unknown command" << std::endl;
		}
	}

	return 0;
}