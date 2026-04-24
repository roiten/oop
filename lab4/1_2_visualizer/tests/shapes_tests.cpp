#include "../src/shapes/CCircle.h"
#include "../src/shapes/CLineSegment.h"
#include "../src/shapes/CRectangle.h"
#include "../src/shapes/CTriangle.h"
#include "../src/shapes/Point.h"
#include "../src/shapes/ISolidShape.h"
#include "../src/shapes/IShape.h"


#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"

#include <cmath>
#include <numbers>
#include <stdexcept>

using Color = uint32_t;

TEST_CASE("Circle")
{
    GIVEN("A circle with center (10, 20) and radius 5")
    {
        CPoint center{10.0, 20.0};
        double radius = 5.0;
        Color fill = 0xFF00FF00;
        Color outline = 0xFFFF0000;

        CCircle circle(center, radius, fill, outline);

        WHEN("Request center and radius")
        {
            THEN("They match initial values")
            {
                REQUIRE(circle.GetCenter().x == center.x);
                REQUIRE(circle.GetCenter().y == center.y);
                REQUIRE(circle.GetRadius() == radius);
            }
        }

        WHEN("Calculate area")
        {
            THEN("It equals pi*r^2")
            {
                REQUIRE(circle.GetArea() == Catch::Approx(std::numbers::pi * radius * radius));
            }
        }

        WHEN("Calculate perimeter")
        {
            THEN("It equals 2pi*r")
            {
                REQUIRE(circle.GetPerimeter() == Catch::Approx(2 * std::numbers::pi * radius));
            }
        }
    }

    GIVEN("A valid center point")
    {
        CPoint center{10.0, 20.0};
        Color fill = 0xFF00FF00;
        Color outline = 0xFFFF0000;

        WHEN("Radius is negative")
        {
            THEN("Exception is thrown")
            {
                REQUIRE_THROWS_AS(CCircle(center, -1.0, fill, outline), std::invalid_argument);
            }
        }

        WHEN("Radius is zero")
        {
            THEN("Exception is thrown")
            {
                REQUIRE_THROWS_AS(CCircle(center, 0.0, fill, outline), std::invalid_argument);
            }
        }
    }
}

TEST_CASE("Line segment")
{
    GIVEN("A line from (1,2) to (4,6)")
    {
        CPoint a{1.0, 2.0};
        CPoint b{4.0, 6.0};
        Color outline = 0xFFFF0000;

        CLineSegment line(a, b, outline);

        WHEN("We calculate area")
        {
            THEN("Area is zero")
            {
                REQUIRE(line.GetArea() == 0.0);
            }
        }

        WHEN("We calculate length")
        {
            double expected = std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));

            THEN("It equals distance between points")
            {
                REQUIRE(line.GetPerimeter() == Catch::Approx(expected));
            }
        }
    }

    GIVEN("Special cases of a line")
    {
        Color outline = 0xFFFF0000;

        WHEN("Line is vertical")
        {
            CLineSegment line(CPoint{1,1}, CPoint{1,5}, outline);

            THEN("Length is correct")
            {
                REQUIRE(line.GetPerimeter() == Catch::Approx(4.0));
            }
        }

        WHEN("Line degenerates to a point")
        {
            CPoint p{1.0, 2.0};
            CLineSegment line(p, p, outline);

            THEN("lenght is zero")
            {
                REQUIRE(line.GetPerimeter() == 0.0);
            }
        }
    }
}

TEST_CASE("Rectangle")
{
    GIVEN("Rectangle from (1,4) to (5,1)")
    {
        CPoint tl{1.0, 4.0};
        CPoint br{5.0, 1.0};

        double width = std::abs(br.x - tl.x);
        double height = std::abs(tl.y - br.y);

        CRectangle rect(tl, width, height, 0xFFFF0000, 0xFF00FF00);

        WHEN("We get dimensions")
        {
            THEN("Width = 4, Height = 3")
            {
                REQUIRE(rect.GetWidth() == Catch::Approx(4.0));
                REQUIRE(rect.GetHeight() == Catch::Approx(3.0));
            }
        }

        WHEN("We calculate area")
        {
            THEN("Area is 12")
            {
                REQUIRE(rect.GetArea() == Catch::Approx(12.0));
            }
        }

        WHEN("We calculate perimeter")
        {
            THEN("Perimeter is 14")
            {
                REQUIRE(rect.GetPerimeter() == Catch::Approx(14.0));
            }
        }
    }

    GIVEN("Degenerate rectangle cases")
    {
        Color fill = 0xFF00FF00;
        Color outline = 0xFFFF0000;

        WHEN("Width is zero")
        {
            CRectangle rect(CPoint{2,4}, 0.0, 3.0, outline, fill);

            THEN("Area is zero and perimeter is based on height")
            {
                REQUIRE(rect.GetWidth() == 0.0);
                REQUIRE(rect.GetHeight() == 3.0);
                REQUIRE(rect.GetArea() == 0.0);
                REQUIRE(rect.GetPerimeter() == Catch::Approx(6.0));
            }
        }

        WHEN("Height is zero")
        {
            CRectangle rect(CPoint{4,4}, 5.0, 0.0, outline, fill);

            THEN("Area is zero and perimeter is based on width")
            {
                REQUIRE(rect.GetWidth() == 5.0);
                REQUIRE(rect.GetHeight() == 0.0);
                REQUIRE(rect.GetArea() == 0.0);
                REQUIRE(rect.GetPerimeter() == Catch::Approx(10.0));
            }
        }

    	WHEN("Height is negative")
        {
        	THEN("Exception is thrown")
        	{
        		REQUIRE_THROWS_AS(CRectangle(CPoint{4,4}, 5.0, -10.0, outline, fill), std::invalid_argument);
        	}
        }

    	WHEN("Width is negative")
        {
        	THEN("Exception is thrown")
        	{
        		REQUIRE_THROWS_AS(CRectangle(CPoint{4,4}, -5.0, 2.0, outline, fill), std::invalid_argument);
        	}
        }

        WHEN("Rectangle is a point")
        {
            CRectangle rect(CPoint{2,3}, 0.0, 0.0, outline, fill);

            THEN("Area and perimeter are zero")
            {
                REQUIRE(rect.GetArea() == 0.0);
                REQUIRE(rect.GetPerimeter() == 0.0);
            }
        }
    }
}

TEST_CASE("Triangle")
{
    GIVEN("Right triangle (0,0), (3,0), (0,4)")
    {
        CTriangle t(CPoint{0,0}, CPoint{3,0}, CPoint{0,4}, 0, 0);

        WHEN("We calculate area")
        {
            THEN("It is 6")
            {
                REQUIRE(t.GetArea() == Catch::Approx(6.0));
            }
        }

        WHEN("We calculate perimeter")
        {
            THEN("It is 12")
            {
                REQUIRE(t.GetPerimeter() == Catch::Approx(12.0));
            }
        }
    }

    GIVEN("Degenerate triangle cases")
    {
        WHEN("All points lie on one line")
        {
            CTriangle t(CPoint{0,0}, CPoint{1,1}, CPoint{2,2}, 0, 0);

            THEN("Area is zero")
            {
                REQUIRE(t.GetArea() == 0.0);
            }
        }

        WHEN("Two points coincide")
        {
            CTriangle t(CPoint{0,0}, CPoint{0,0}, CPoint{1,1}, 0, 0);

            THEN("Area is zero")
            {
                REQUIRE(t.GetArea() == 0.0);
            }
        }
    }
}

