#define _USE_MATH_DEFINES

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

struct EquationRoot4 {
    double numRoots;
    double roots[4];
};

const double EPS = 1e-9;

void Solve2(double a, double b, double c, double ans[], int& counter)
{
    if (std::abs(a) < EPS)
    {
        if (std::abs(b) > EPS)
            ans[counter++] = -c / b;
        return;
    }
    double D = b * b - 4.0 * a * c;
    if (D < -EPS) return;
    if (D < 0) D = 0;
    double sq = std::sqrt(D);
    ans[counter++] = (-b - sq) / (2.0 * a);
    if (sq > EPS)
        ans[counter++] = (-b + sq) / (2.0 * a);
}

int SolveDepressed3(double p, double q, double roots[3])
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
        double r   = std::sqrt(-p * p * p / 27.0);
        double arg = std::max(-1.0, std::min(1.0, -q / (2.0 * r)));
        double phi = std::acos(arg);
        double m   = 2.0 * std::cbrt(r);
        roots[0] = m * std::cos(phi / 3.0);
        roots[1] = m * std::cos((phi + 2.0 * M_PI) / 3.0);
        roots[2] = m * std::cos((phi + 4.0 * M_PI) / 3.0);
        return 3;
    }
}

EquationRoot4 Solve4(double a, double b, double c, double d, double e)
{
    if (std::abs(a) < EPS)
        throw std::invalid_argument("Coefficient a must not be zero");

    const double B  = b / a, C = c / a, D = d / a, E = e / a;
    const double s  = -B / 4.0;

    const double k2 = C - 3.0 * B * B / 8.0;
    const double k1 = B * B * B / 8.0 - B * C / 2.0 + D;
    const double k0 = -3.0 * B * B * B * B / 256.0 + B * B * C / 16.0 - B * D / 4.0 + E;

    double dirtyRoots[4];
    int counter = 0;

    if (std::abs(k1) < EPS)
    {
        double Dq = k2 * k2 - 4.0 * k0;
        if (Dq < -EPS)
            throw std::domain_error("The degree-4 equation has no real roots");
        if (Dq < 0) Dq = 0;
        double sq = std::sqrt(Dq);

        for (double u : { (-k2 - sq) / 2.0, (-k2 + sq) / 2.0 })
        {
            if (u < -EPS) continue;
            double t = std::sqrt(std::max(0.0, u));
            if (t < EPS)
                dirtyRoots[counter++] = 0.0;
            else
            {
                dirtyRoots[counter++] = -t;
                dirtyRoots[counter++] =  t;
            }
        }
    }
    else
    {
        const double pc = -(k2 * k2) / 3.0 - 4.0 * k0;
        const double qc =  2.0 * k2 * k2 * k2 / 27.0 + 4.0 * k0 * k2 / 3.0 - k1 * k1;

        double cubRoots[3];
        int nCub = SolveDepressed3(pc, qc, cubRoots);

        for (int i = 0; i < nCub; ++i)
            cubRoots[i] += k2 / 3.0;

        double R = cubRoots[0];

        double disc = R * R - 4.0 * k0;
        if (disc < 0.0) disc = 0.0;

        double S = std::sqrt(disc);

        if (S < EPS)
            throw std::domain_error("The degree-4 equation has no real roots");


        double denom_part1 = S + R;
        double denom_part2 = R * S - 4.0 * k0 + R * R;
        double full_denom = denom_part1 * denom_part2;

        if (std::abs(full_denom) < EPS || std::abs(denom_part1) < EPS)
            throw std::domain_error("Division by zero in the ftvmetrics analytical formula");

        double numerator = 2.0 * k1 * (R * S - 2.0 * k0 + R * R);

        double F = numerator / full_denom;

        double c1 = 0.5 * denom_part1 - F;
        double c2 = (2.0 * k0) / denom_part1 + F;

        Solve2(1.0,  S, c1, dirtyRoots, counter);
        Solve2(1.0, -S, c2, dirtyRoots, counter);
    }

    if (counter == 0)
        throw std::domain_error("The degree-4 equation has no real roots");

    for (int i = 0; i < counter; ++i)
        dirtyRoots[i] += s;

    std::sort(dirtyRoots, dirtyRoots + counter);
    EquationRoot4 result;
    result.numRoots = 0;

    result.roots[static_cast<int>(result.numRoots++)] = dirtyRoots[0];
    for (int i = 1; i < counter; ++i) {
        if (std::abs(dirtyRoots[i] - dirtyRoots[i - 1]) > EPS) {
            result.roots[static_cast<int>(result.numRoots++)] = dirtyRoots[i];
        }
    }

    return result;
}