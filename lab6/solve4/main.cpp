#include "solve4/solve4.h"
#include <iostream>
#include <stdexcept>

int main()
{
	std::cout << "Enter coefficients a b c d e of ax^4+bx^3+cx^2+dx+e=0\n"
			  << "(Ctrl+Z to exit):\n";

	double a, b, c, d, e;
	while (std::cin >> a >> b >> c >> d >> e)
	{
		try
		{
			EquationRoot4 result = Solve4(a, b, c, d, e);
			std::cout << "Number of real roots: " << result.numRoots << "\n";
			for (int i = 0; i < result.numRoots; ++i)
				std::cout << "x" << (i + 1) << " = " << result.roots[i] << "\n";
		}
		catch (const std::invalid_argument& ex)
		{
			std::cout << "Invalid argument: " << ex.what() << "\n";
		}
		catch (const std::domain_error& ex)
		{
			std::cout << "No real roots: " << ex.what() << "\n";
		}
	}

	return 0;
}