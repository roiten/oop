#include "../solve4/solve4.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cmath>
#include <stdexcept>

using namespace Catch::Matchers;

static double Eval4(double a, double b, double c, double d, double e, double x)
{
    return ((((a * x) + b) * x + c) * x + d) * x + e;
}

static bool RootsEqual(const auto& r, const std::vector<double>& expected)
{
    if (r.numRoots != expected.size())
        return false;
    for (int i = 0; i < r.numRoots; ++i)
    {
        if (std::abs(r.roots[i] - expected[i]) > 1e-6)
            return false;
    }
    return true;
}

static void RequireRootValid(double a, double b, double c, double d, double e, double x, double eps = 1e-6)
{
    REQUIRE_THAT(Eval4(a, b, c, d, e, x), WithinAbs(0.0, eps));
}

// Tests for Solve2 (quadratic equation solver)
TEST_CASE("Solve2 handles quadratic equation with two distinct roots")
{
    GIVEN("Quadratic equation x^2 - 5x + 6 = 0 (roots 2 and 3)")
    {
        double buf[2];
        int counter = 0;

        WHEN("Solve2 is called")
        {
            Solve2(1.0, -5.0, 6.0, buf, counter);

            THEN("It finds 2 roots")
            {
                REQUIRE(counter == 2);
            }

            THEN("Roots are 2 and 3")
            {
                REQUIRE_THAT(buf[0], WithinAbs(2.0, 1e-6));
                REQUIRE_THAT(buf[1], WithinAbs(3.0, 1e-6));
            }
        }
    }
}

TEST_CASE("Solve2 handles quadratic equation with one double root")
{
    GIVEN("Quadratic equation x^2 - 4x + 4 = 0 (double root at 2)")
    {
        double buf[2];
        int counter = 0;

        WHEN("Solve2 is called")
        {
            Solve2(1.0, -4.0, 4.0, buf, counter);

            THEN("It finds 1 root")
            {
                REQUIRE(counter == 1);
            }

            THEN("Root is 2")
            {
                REQUIRE_THAT(buf[0], WithinAbs(2.0, 1e-6));
            }
        }
    }
}

TEST_CASE("Solve2 handles linear equation when a is near zero")
{
    GIVEN("Equation 2x + 3 = 0 (a ~ 0, linear case)")
    {
        double buf[2];
        int counter = 0;

        WHEN("Solve2 is called with a near zero")
        {
            Solve2(1e-10, 2.0, 3.0, buf, counter);

            THEN("It finds 1 root from linear equation")
            {
                REQUIRE(counter == 1);
                REQUIRE_THAT(buf[0], WithinAbs(-1.5, 1e-6));
            }
        }
    }
}

TEST_CASE("Solve2 handles quadratic with no real roots")
{
    GIVEN("Quadratic equation x^2 + 1 = 0 (no real roots)")
    {
        double buf[2];
        int counter = 0;

        WHEN("Solve2 is called")
        {
            Solve2(1.0, 0.0, 1.0, buf, counter);

            THEN("It finds 0 roots")
            {
                REQUIRE(counter == 0);
            }
        }
    }
}

TEST_CASE("Solve3 with one real root")
{
    GIVEN("Cubic t^3 + t + 1 = 0 (p=1, q=1)")
    {
        double roots[3];

        WHEN("Solve3 is called")
        {
            int numRoots = Solve3(1.0, 1.0, roots);

            THEN("It returns 1 real root")
            {
                REQUIRE(numRoots == 1);
            }

            THEN("The root satisfies the equation approx")
            {
                double r = roots[0];
                REQUIRE_THAT(r*r*r + r + 1.0, WithinAbs(0.0, 1e-6));
            }
        }
    }
}

TEST_CASE("Solve3 cubic with three real roots")
{
    GIVEN("Depressed cubic t^3 - 3t + 1 = 0 (p=-3, q=1)")
    {
        double roots[3];

        WHEN("Solve3 is called")
        {
            int numRoots = Solve3(-3.0, 1.0, roots);

            THEN("It returns 3 real roots")
            {
                REQUIRE(numRoots == 3);
            }

            THEN("All roots satisfy the equation")
            {
                for (int i = 0; i < numRoots; ++i)
                {
                    double r = roots[i];
                    REQUIRE_THAT(r*r*r - 3.0*r + 1.0, WithinAbs(0.0, 1e-6));
                }
            }

            THEN("Roots are in ascending order")
            {
                REQUIRE(roots[0] <= roots[1]);
                REQUIRE(roots[1] <= roots[2]);
            }
        }
    }
}

TEST_CASE("Solve3 cubic with double root")
{
    GIVEN("Depressed cubic t^3 - 3t + 2 = 0 (p=-3, q=2)")
    {
        double roots[3];

        WHEN("Solve3 is called")
        {
            int numRoots = Solve3(-3.0, 2.0, roots);

            THEN("It returns 2 distinct roots (one double)")
            {
                REQUIRE(numRoots == 2);
            }

            THEN("The roots are -2 and 1")
            {
                REQUIRE_THAT(roots[0], WithinAbs(-2.0, 1e-6));
                REQUIRE_THAT(roots[1], WithinAbs(1.0, 1e-6));
            }
        }
    }
}

TEST_CASE("Solve4 exception when coefficient a is zero")
{
    GIVEN("The coefficient a is zero")
    {
        WHEN("Solve4 is called")
        {
            THEN("It throws invalid_argument")
            {
                REQUIRE_THROWS_AS(Solve4(0, 1, 2, 3, 4), std::invalid_argument);
            }
        }
    }
}

TEST_CASE("Solve4 throws domain_error when no real roots exist")
{
    GIVEN("equation without real roots (x^4 + 1 = 0)")
    {
        WHEN("Solve4 is called")
        {
            THEN("It throws domain_error")
            {
                REQUIRE_THROWS_AS(Solve4(1, 0, 0, 0, 1), std::domain_error);
            }
        }
    }
}

TEST_CASE("Solve4 throws domain_error for x^4 + x^2 + 1")
{
    GIVEN("An equation x^4 + x^2 + 1 = 0 with no real roots")
    {
        WHEN("Solve4 is called")
        {
            THEN("It throws domain_error")
            {
                REQUIRE_THROWS_AS(Solve4(1, 0, 1, 0, 1), std::domain_error);
            }
        }
    }
}

TEST_CASE("Solve4 finds four distinct roots")
{
    GIVEN("Equation (x-1)(x-2)(x-3)(x-4) = x^4 - 10x^3 + 35x^2 - 50x + 24")
    {
        WHEN("Solve4 is called")
        {
            auto r = Solve4(1, -10, 35, -50, 24);
            
            THEN("It returns 4 roots")
            {
                REQUIRE(r.numRoots == 4);
            }
            
            THEN("Roots are 1, 2, 3, 4")
            {
                // REQUIRE_THAT(r.roots[0], WithinAbs(1.0, 1e-6));
                // REQUIRE_THAT(r.roots[1], WithinAbs(2.0, 1e-6));
                // REQUIRE_THAT(r.roots[2], WithinAbs(3.0, 1e-6));
                // REQUIRE_THAT(r.roots[3], WithinAbs(4.0, 1e-6));
            	REQUIRE(RootsEqual(r, std::vector<double>{1, 2, 3, 4}));
            }
            
            THEN("Each root satisfies the equation")
            {
                for (int i = 0; i < r.numRoots; ++i)
                    RequireRootValid(1, -10, 35, -50, 24, r.roots[i]);
            }
        }
    }
}

TEST_CASE("Solve4 finds four distinct negative roots -4, -3, -2, -1")
{
    GIVEN("An equation (x+1)(x+2)(x+3)(x+4) = x^4 + 10x^3 + 35x^2 + 50x + 24")
    {
        WHEN("Solve4 is called")
        {
            auto r = Solve4(1, 10, 35, 50, 24);
            
            THEN("It returns 4 roots")
            {
                REQUIRE(r.numRoots == 4);
            }
            
            THEN("Roots are -4, -3, -2, -1")
            {
                REQUIRE_THAT(r.roots[0], WithinAbs(-4.0, 1e-6));
                REQUIRE_THAT(r.roots[1], WithinAbs(-3.0, 1e-6));
                REQUIRE_THAT(r.roots[2], WithinAbs(-2.0, 1e-6));
                REQUIRE_THAT(r.roots[3], WithinAbs(-1.0, 1e-6));
            }
            
            THEN("Each root satisfies the equation")
            {
                for (int i = 0; i < r.numRoots; ++i)
                    RequireRootValid(1, 10, 35, 50, 24, r.roots[i]);
            }
        }
    }
}

TEST_CASE("Solve4 finds four symmetric roots -2, -1, 1, 2")
{
    GIVEN("An equation (x-1)(x+1)(x-2)(x+2) = x^4 - 5x^2 + 4")
    {
        WHEN("Solve4 is called")
        {
            auto r = Solve4(1, 0, -5, 0, 4);
            
            THEN("It returns 4 roots")
            {
                REQUIRE(r.numRoots == 4);
            }
            
            THEN("Roots are -2, -1, 1, 2")
            {
                REQUIRE_THAT(r.roots[0], WithinAbs(-2.0, 1e-6));
                REQUIRE_THAT(r.roots[1], WithinAbs(-1.0, 1e-6));
                REQUIRE_THAT(r.roots[2], WithinAbs(1.0, 1e-6));
                REQUIRE_THAT(r.roots[3], WithinAbs(2.0, 1e-6));
            }
            
            THEN("Each root satisfies the equation")
            {
                for (int i = 0; i < r.numRoots; ++i)
                    RequireRootValid(1, 0, -5, 0, 4, r.roots[i]);
            }
        }
    }
}

TEST_CASE("Solve4 finds two roots for x^4 - 1 = 0")
{
    GIVEN("An equation x^4 - 1 = 0")
    {
        WHEN("Solve4 is called")
        {
            auto r = Solve4(1, 0, 0, 0, -1);
            
            THEN("It returns 2 roots")
            {
                REQUIRE(r.numRoots == 2);
            }
            
            THEN("Roots are -1 and 1")
            {
                REQUIRE_THAT(r.roots[0], WithinAbs(-1.0, 1e-6));
                REQUIRE_THAT(r.roots[1], WithinAbs(1.0, 1e-6));
            }
            
            THEN("Each root satisfies the equation")
            {
                for (int i = 0; i < r.numRoots; ++i)
                    RequireRootValid(1, 0, 0, 0, -1, r.roots[i]);
            }
        }
    }
}

TEST_CASE("Solve4 finds one repeated root of multiplicity 4 at x = 3")
{
    GIVEN("An equation (x-3)^4 = x^4 - 12x^3 + 54x^2 - 108x + 81")
    {
        WHEN("Solve4 is called")
        {
            auto r = Solve4(1, -12, 54, -108, 81);
            
            THEN("It returns at least 1 root")
            {
                REQUIRE(r.numRoots >= 1);
            }
            
            THEN("The root is approximately 3")
            {
                bool found = false;
                for (int i = 0; i < r.numRoots; ++i)
                {
                    if (std::abs(r.roots[i] - 3.0) < 1e-4)
                        found = true;
                }
                REQUIRE(found);
            }
            
            THEN("Each root satisfies the equation")
            {
                for (int i = 0; i < r.numRoots; ++i)
                    RequireRootValid(1, -12, 54, -108, 81, r.roots[i], 1e-3);
            }
        }
    }
}

TEST_CASE("Solve4 finds two double roots at 1 and 2")
{
    GIVEN("An equation (x-1)^2 * (x-2)^2 = x^4 - 6x^3 + 13x^2 - 12x + 4")
    {
        WHEN("Solve4 is called")
        {
            auto r = Solve4(1, -6, 13, -12, 4);
            
            THEN("It returns 2 roots")
            {
                REQUIRE(r.numRoots == 2);
            }
            
            THEN("Roots are 1 and 2")
            {
                REQUIRE_THAT(r.roots[0], WithinAbs(1.0, 1e-5));
                REQUIRE_THAT(r.roots[1], WithinAbs(2.0, 1e-5));
            }
            
            THEN("Each root satisfies the equation")
            {
                for (int i = 0; i < r.numRoots; ++i)
                    RequireRootValid(1, -6, 13, -12, 4, r.roots[i], 1e-4);
            }
        }
    }
}

TEST_CASE("Solve4 throws domain_error for x^4 + 5x^2 + 4")
{
    GIVEN("An equation (x^2+1)(x^2+4) = x^4 + 5x^2 + 4")
    {
        WHEN("Solve4 is called")
        {
            THEN("It throws domain_error (no real roots)")
            {
                REQUIRE_THROWS_AS(Solve4(1, 0, 5, 0, 4), std::domain_error);
            }
        }
    }
}

TEST_CASE("Solve4 handles scaled coefficients correctly")
{
    GIVEN("An equation scaled by factor 2: 2*(x-1)(x-2)(x-3)(x-4)")
    {
        WHEN("Solve4 is called with scaled coefficients")
        {
            auto r = Solve4(2, -20, 70, -100, 48);
            
            THEN("It returns 4 roots")
            {
                REQUIRE(r.numRoots == 4);
            }
            
            THEN("Roots are still 1, 2, 3, 4")
            {
                REQUIRE_THAT(r.roots[0], WithinAbs(1.0, 1e-5));
                REQUIRE_THAT(r.roots[1], WithinAbs(2.0, 1e-5));
                REQUIRE_THAT(r.roots[2], WithinAbs(3.0, 1e-5));
                REQUIRE_THAT(r.roots[3], WithinAbs(4.0, 1e-5));
            }
            
            THEN("Each root satisfies the equation")
            {
                for (int i = 0; i < r.numRoots; ++i)
                    RequireRootValid(2, -20, 70, -100, 48, r.roots[i]);
            }
        }
    }
}

TEST_CASE("Solve4 handles equation x^4 = 0")
{
    GIVEN("An equation x^4 = 0")
    {
        WHEN("Solve4 is called")
        {
            auto r = Solve4(1, 0, 0, 0, 0);
            
            THEN("It returns at least 1 root")
            {
                REQUIRE(r.numRoots >= 1);
            }
            
            THEN("The root is 0")
            {
                REQUIRE_THAT(r.roots[0], WithinAbs(0.0, 1e-9));
            }
        }
    }
}

TEST_CASE("Solve4 handles negative coefficient a")
{
    GIVEN("An equation with negative a: -(x-1)(x-2)(x-3)(x-4)")
    {
        WHEN("Solve4 is called")
        {
            auto r = Solve4(-1, 10, -35, 50, -24);
            
            THEN("It returns 4 roots")
            {
                REQUIRE(r.numRoots == 4);
            }
            
            THEN("Roots are still 1, 2, 3, 4")
            {
                REQUIRE_THAT(r.roots[0], WithinAbs(1.0, 1e-5));
                REQUIRE_THAT(r.roots[1], WithinAbs(2.0, 1e-5));
                REQUIRE_THAT(r.roots[2], WithinAbs(3.0, 1e-5));
                REQUIRE_THAT(r.roots[3], WithinAbs(4.0, 1e-5));
            }
        }
    }
}

TEST_CASE("Solve4 returns roots in ascending order")
{
    GIVEN("An equation with four distinct roots 1, 2, 3, 4")
    {
        WHEN("Solve4 is called")
        {
            auto r = Solve4(1, -10, 35, -50, 24);
            
            THEN("Roots are sorted in ascending order")
            {
                for (int i = 1; i < r.numRoots; ++i)
                    REQUIRE(r.roots[i - 1] <= r.roots[i]);
            }
        }
    }
}

