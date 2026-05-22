#include "shapes/CCircle.h"
#include "shapes/CLineSegment.h"
#include "shapes/CRectangle.h"
#include "shapes/CTriangle.h"
#include "shapes/IShape.h"

#include <algorithm>
#include <ranges>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

uint32_t ParseColor(const std::string& hexStr)
{
    try {
        return static_cast<uint32_t>(std::stoul(hexStr, nullptr, 16));
    } catch (...) {
    	std::cerr << "Incorrect color format" << std::endl;
        return 0;
    }
}

//вынести получение перимтра и плаощади и протестировать
std::shared_ptr<IShape> FindShapeWithMaxArea(const std::vector<std::shared_ptr<IShape>>& shapes)
{
    if (shapes.empty()) return nullptr;
    return *std::ranges::max_element(shapes,
        [](const auto& shape1, const auto& shape2) { return shape1->GetArea() < shape2->GetArea(); });
}

std::shared_ptr<IShape> FindShapeWithMinPerimeter(const std::vector<std::shared_ptr<IShape>>& shapes)
{
    if (shapes.empty()) return nullptr;
    return *std::ranges::min_element(shapes,
        [](const auto& s1, const auto& s2) { return s1->GetPerimeter() < s2->GetPerimeter(); });
}

int main()
{
    std::vector<std::shared_ptr<IShape>> shapes;
    std::string line;

    while (std::getline(std::cin, line))
    {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string shapeType;
        iss >> shapeType;

        try
        {
            if (shapeType == "rectangle")
            {
                double x, y, w, h;
                std::string outlineStr, fillStr;
                iss >> x >> y >> w >> h >> outlineStr >> fillStr;
                shapes.push_back(std::make_shared<CRectangle>(
                    CPoint{x, y}, w, h, ParseColor(outlineStr), ParseColor(fillStr)));
            }
            else if (shapeType == "circle")
            {
                double x, y, r;
                std::string outlineStr, fillStr;
                iss >> x >> y >> r >> outlineStr >> fillStr;
                shapes.push_back(std::make_shared<CCircle>(
                    CPoint{x, y}, r, ParseColor(outlineStr), ParseColor(fillStr)));
            }
            else if (shapeType == "triangle")
            {
                double x1, y1, x2, y2, x3, y3;
                std::string outlineStr, fillStr;
                iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> outlineStr >> fillStr;
                shapes.push_back(std::make_shared<CTriangle>(
                    CPoint{x1, y1}, CPoint{x2, y2}, CPoint{x3, y3}, ParseColor(outlineStr), ParseColor(fillStr)));
            }
            else if (shapeType == "line")
            {
                double x1, y1, x2, y2;
                std::string outlineStr;
                iss >> x1 >> y1 >> x2 >> y2 >> outlineStr;
                shapes.push_back(std::make_shared<CLineSegment>(
                    CPoint{x1, y1}, CPoint{x2, y2}, ParseColor(outlineStr)));
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error parsing line: " << line << "\n" << e.what() << "\n";
        }
    }

    if (shapes.empty())
    {
        std::cout << "No shapes were entered.\n";
        return 0;
    }

    auto maxAreaShape = FindShapeWithMaxArea(shapes);
    auto minPerimeterShape = FindShapeWithMinPerimeter(shapes);

    std::cout << "Shape with maximum area" << std::endl;
    std::cout << maxAreaShape->ToString() << std::endl;

    std::cout << "\n Shape with minimum perimeter:" << std::endl;
    std::cout << minPerimeterShape->ToString() << std::endl;

    return 0;
}