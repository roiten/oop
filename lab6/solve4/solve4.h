#pragma once
#include <algorithm>
#include <cmath>
#include <stdexcept>

const double EPS = 1e-9;

struct EquationRoot4
{
	int numRoots;
	double roots[4];
};

static inline void Solve2(double a, double b, double c, double buf[], int& counter)
{
	if (std::abs(a) < EPS)
	{
		if (std::abs(b) > EPS)
			buf[counter++] = -c / b;
		return;
	}
	double D = b * b - 4.0 * a * c;
	if (D < -EPS)
		return;
	if (D < 0)
		D = 0;
	double sq = std::sqrt(D);
	buf[counter++] = (-b - sq) / (2.0 * a);
	if (sq > EPS)
		buf[counter++] = (-b + sq) / (2.0 * a);
}

static inline int Solve3(double p, double q, double roots[3])
{
	double D = q * q / 4.0 + p * p * p / 27.0;

	if (D > EPS)
	{
		double sq = std::sqrt(D);
		roots[0] = std::cbrt(-q / 2.0 + sq) + std::cbrt(-q / 2.0 - sq);
		return 1;
	}
	else if (D > -EPS)
	{
		double u = std::cbrt(-q / 2.0);
		roots[0] = 2.0 * u;
		roots[1] = -u;
		return (std::abs(roots[0] - roots[1]) < EPS) ? 1 : 2;
	}
	else
	{
		double r = std::sqrt(-p * p * p / 27.0);
		double arg = std::max(-1.0, std::min(1.0, -q / (2.0 * r)));
		double phi = std::acos(arg);
		double m = 2.0 * std::cbrt(r);
		roots[0] = m * std::cos(phi / 3.0);
		roots[1] = m * std::cos((phi + 2.0 * M_PI) / 3.0);
		roots[2] = m * std::cos((phi + 4.0 * M_PI) / 3.0);
		return 3;
	}
}

/**
 * Вычисляет вещественные корни уравнения 4 степени методом Феррари.
 *
 * @throws std::invalid_argument  если a == 0
 * @throws std::domain_error      если вещественных корней нет
 */
inline EquationRoot4 Solve4(double a, double b, double c, double d, double e)
{
	if (std::abs(a) < EPS)
		throw std::invalid_argument("Coefficient a must not be zero for a degree-4 equation");

	const double B = b / a, C = c / a, D = d / a, E = e / a;

	const double s = -B / 4.0;
	const double p = -3.0 * B * B / 8.0 + C;
	const double q = B * B * B / 8.0 - B * C / 2.0 + D;
	const double r = -3.0 * B * B * B * B / 256.0 + B * B * C / 16.0 - B * D / 4.0 + E;

	double buf[4];
	int counter = 0;

	if (std::abs(q) < EPS)
	{
		double Dq = p * p - 4.0 * r;

		if (Dq < -EPS)
			throw std::domain_error("The degree-4 equation has no real roots");

		if (Dq < 0) Dq = 0;

		double sq = std::sqrt(Dq);
		double u1 = (-p - sq) / 2.0;
		double u2 = (-p + sq) / 2.0;
		for (double u : { u1, u2 })
		{
			if (u < -EPS) continue;
			double t = std::sqrt(std::max(0.0, u));
			if (t < EPS)
			{
				buf[counter++] = 0.0;
			}
			else
			{
				buf[counter++] = -t;
				buf[counter++] = t;
			}
		}
	}
	else
	{
		const double pa = -p / 2.0;
		const double pb = -r;
		const double pc = p * r / 2.0 - q * q / 8.0;
		const double ysh = pa / 3.0;
		const double pp2 = pb - pa * pa / 3.0;
		const double qq2 = 2.0 * pa * pa * pa / 27.0 - pa * pb / 3.0 + pc;

		double cubRoots[3];
		int nCub = Solve3(pp2, qq2, cubRoots);
		for (int i = 0; i < nCub; ++i)
		{
			cubRoots[i] -= ysh;
		}

		double y = cubRoots[0];
		for (int i = 1; i < nCub; ++i)
		{
			if (2.0 * cubRoots[i] - p > 2.0 * y - p)
				y = cubRoots[i];
		}

		double m = 2.0 * y - p;
		if (m < 0.0)
			m = 0.0;

		double sqrtM = std::sqrt(m);

		if (sqrtM < EPS)
		{
			throw std::domain_error("The degree-4 equation has no real roots");
		}

		double h = q / (2.0 * sqrtM);
		Solve2(1.0, sqrtM, y + h, buf, counter);
		Solve2(1.0, -sqrtM, y - h, buf, counter);
	}

	if (counter == 0)
	{
		throw std::domain_error("The degree-4 equation has no real roots");
	}

	for (int i = 0; i < counter; ++i)
		buf[i] += s;

	std::sort(buf, buf + counter);
	EquationRoot4 result{ 0, {} };
	result.roots[result.numRoots++] = buf[0];
	for (int i = 1; i < counter; ++i)
		if (std::abs(buf[i] - buf[i - 1]) > EPS)
			result.roots[result.numRoots++] = buf[i];

	return result;
}