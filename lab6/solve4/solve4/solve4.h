struct EquationRoot4
{
	int numRoots;
	double roots[4];
};

void Solve2(double a, double b, double c, double ans[], int& counter);
int SolveDepressed3(double p, double q, double roots[3]);
EquationRoot4 Solve4(double a, double b, double c, double d, double e);
