#include "../src/shapes/CCircle.h"
#include "../src/shapes/CLineSegment.h"
#include "../src/shapes/CRectangle.h"
#include "../src/shapes/CTriangle.h"
#include "../src/shapes/Point.h"
#include "../src/canvas/ICanvas.h"

#include "catch2/catch_test_macros.hpp"
#include <fakeit.hpp>

#include <cmath>
#include <numbers>
#include <stdexcept>
#include <vector>

using Color = uint32_t;
using namespace fakeit;

bool PointsEqual(const CPoint& a, const CPoint& b)
{
    return a.x == b.x && a.y == b.y;
}

//добавить проверку на использование только нужных методов

TEST_CASE("CCircle Draw method")
{
    GIVEN("A circle with center (10, 20) and radius 5")
    {
        CPoint center{10.0, 20.0};
        double radius = 5.0;
        Color outlineColor = 0xFFFF0000;
        Color fillColor = 0xFF00FF00;

        CCircle circle(center, radius, outlineColor, fillColor);

        WHEN("Draw is called")
        {
            Mock<ICanvas> canvasMock;
            Fake(Method(canvasMock, FillCircle));
            Fake(Method(canvasMock, DrawCircle));

            circle.Draw(canvasMock.get());

            THEN("It calls FillCircle and DrawCircle with correct parameters")
            {
                Verify(Method(canvasMock, FillCircle)
                    .Matching([&](const CPoint& c, double r, Color col) {
                        return PointsEqual(c, center) && r == radius && col == fillColor;
                    }))
                    .Once();
                Verify(Method(canvasMock, DrawCircle)
                    .Matching([&](const CPoint& c, double r, Color col) {
                        return PointsEqual(c, center) && r == radius && col == outlineColor;
                    }))
                    .Once();
            }
        }
    }
}

TEST_CASE("CLineSegment Draw method")
{
    GIVEN("A line from (1,2) to (4,6)")
    {
        CPoint a{1.0, 2.0};
        CPoint b{4.0, 6.0};
        Color outlineColor = 0xFFFF0000;

        CLineSegment line(a, b, outlineColor);

        WHEN("Draw is called")
        {
            Mock<ICanvas> canvasMock;
            Fake(Method(canvasMock, DrawLine));

            line.Draw(canvasMock.get());

            THEN("It calls DrawLine with correct parameters")
            {
                Verify(Method(canvasMock, DrawLine)
                    .Matching([&](const CPoint& from, const CPoint& to, Color col) {
                        return PointsEqual(from, a) && PointsEqual(to, b) && col == outlineColor;
                    }))
                    .Once();
            }
        }
    }
}

TEST_CASE("CRectangle Draw method")
{
    GIVEN("A rectangle at (1,4) with width 4 and height 3")
    {
        CPoint leftTop{1.0, 4.0};
        double width = 4.0;
        double height = 3.0;
        Color outlineColor = 0xFFFF0000;
        Color fillColor = 0xFF00FF00;

        CRectangle rect(leftTop, width, height, outlineColor, fillColor);

        WHEN("Draw is called")
        {
            Mock<ICanvas> canvasMock;
            Fake(Method(canvasMock, FillPolygon));
            Fake(Method(canvasMock, DrawLine));

            rect.Draw(canvasMock.get());

            THEN("It calls FillPolygon and DrawLine for each side")
            {
                Verify(Method(canvasMock, FillPolygon)).Once();
                Verify(Method(canvasMock, DrawLine)).Exactly(4);
            }
        }
    }
}

TEST_CASE("CTriangle Draw method")
{
    GIVEN("A triangle with vertices (0,0), (3,0), (0,4)")
    {
        CPoint v1{0.0, 0.0};
        CPoint v2{3.0, 0.0};
        CPoint v3{0.0, 4.0};
        Color outlineColor = 0xFFFF0000;
        Color fillColor = 0xFF00FF00;

        CTriangle triangle(v1, v2, v3, outlineColor, fillColor);

        WHEN("Draw is called")
        {
            Mock<ICanvas> canvasMock;
            Fake(Method(canvasMock, FillPolygon));
            Fake(Method(canvasMock, DrawLine));

            triangle.Draw(canvasMock.get());

            THEN("It calls FillPolygon and DrawLine for each side")
            {
                Verify(Method(canvasMock, FillPolygon)).Once();
                Verify(Method(canvasMock, DrawLine)).Exactly(3);
            }
        }
    }
}

TEST_CASE("Multiple shapes draw in sequence")
{
    GIVEN("Multiple shapes")
    {
        CCircle circle(CPoint{10,10}, 5, 0xFFFF0000, 0xFF00FF00);
        CLineSegment line(CPoint{0,0}, CPoint{10,10}, 0xFFFF0000);

        WHEN("All shapes are drawn in order")
        {
            Mock<ICanvas> canvasMock;
            Fake(Method(canvasMock, FillCircle));
            Fake(Method(canvasMock, DrawCircle));
            Fake(Method(canvasMock, DrawLine));

            circle.Draw(canvasMock.get());
            line.Draw(canvasMock.get());

            THEN("Each draw method is called appropriate number of times")
            {
                Verify(Method(canvasMock, FillCircle)).Once();
                Verify(Method(canvasMock, DrawCircle)).Once();
                Verify(Method(canvasMock, DrawLine)).Once();
            }
        }
    }
}