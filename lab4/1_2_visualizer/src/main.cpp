#include "canvas/CCanvas.h"
#include "shapes/CCircle.h"
#include "shapes/CLineSegment.h"
#include "shapes/CRectangle.h"
#include "shapes/CTriangle.h"
#include "shapes/FindProperties.h"
#include "shapes/ICanvasDrawable.h"
#include "shapes/IShape.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
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

int main()
{
    std::vector<std::shared_ptr<ICanvasDrawable>> drawableShapes;
    std::string line;

    std::cout << "Enter shapes (rectangle, circle, triangle, line):\n";
    std::cout << "Examples:\n";
    std::cout << "  rectangle 100 100 200 150 FF0000 00FF00\n";
    std::cout << "  circle 300 300 50 0000FF FFFF00\n";
    std::cout << "  triangle 400 400 500 400 450 500 FF00FF 00FFFF\n";
    std::cout << "  line 600 600 700 650 000000\n";
    std::cout << "Press Ctrl+Z (Windows) or Ctrl+D (Linux) to finish input\n\n";

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
                auto shape = std::make_shared<CRectangle>(
                    CPoint{x, y}, w, h, ParseColor(outlineStr), ParseColor(fillStr));
                drawableShapes.push_back(shape);
                std::cout << "Added rectangle at (" << x << ", " << y << ")\n";
            }
            else if (shapeType == "circle")
            {
                double x, y, r;
                std::string outlineStr, fillStr;
                iss >> x >> y >> r >> outlineStr >> fillStr;
                auto shape = std::make_shared<CCircle>(
                    CPoint{x, y}, r, ParseColor(outlineStr), ParseColor(fillStr));
                drawableShapes.push_back(shape);
                std::cout << "Added circle at (" << x << ", " << y << ") with radius " << r << "\n";
            }
            else if (shapeType == "triangle")
            {
                double x1, y1, x2, y2, x3, y3;
                std::string outlineStr, fillStr;
                iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> outlineStr >> fillStr;
                auto shape = std::make_shared<CTriangle>(
                    CPoint{x1, y1}, CPoint{x2, y2}, CPoint{x3, y3},
                    ParseColor(outlineStr), ParseColor(fillStr));
                drawableShapes.push_back(shape);
                std::cout << "Added triangle\n";
            }
            else if (shapeType == "line")
            {
                double x1, y1, x2, y2;
                std::string outlineStr;
                iss >> x1 >> y1 >> x2 >> y2 >> outlineStr;
                auto shape = std::make_shared<CLineSegment>(
                    CPoint{x1, y1}, CPoint{x2, y2}, ParseColor(outlineStr));
                drawableShapes.push_back(shape);
                std::cout << "Added line from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ")\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error parsing line: " << line << "\n" << e.what() << "\n";
        }
    }

    if (drawableShapes.empty())
    {
        std::cout << "No shapes were entered. Exiting.\n";
        return 0;
    }

	std::vector<std::shared_ptr<IShape>> shapes;
	for (const auto& shape : drawableShapes)
	{
		shapes.push_back(std::dynamic_pointer_cast<IShape>(shape));
	}
	auto maxAreaShape = FindShapeWithMaxArea(shapes);
	auto minPerimeterShape = FindShapeWithMinPerimeter(shapes);

	std::cout << "Shape with maximum area" << std::endl;
	std::cout << maxAreaShape->ToString() << std::endl;

	std::cout << "\n Shape with minimum perimeter:" << std::endl;
	std::cout << minPerimeterShape->ToString() << std::endl;

    std::cout << "\nDrawing " << drawableShapes.size() << " shapes...\n";
    std::cout << "Close the window to exit.\n";

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Shapes Visualizer");
    CCanvas canvas(window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        canvas.Clear(0xFFFFFF);

        for (const auto& shape : drawableShapes)
        {
            shape->Draw(canvas);
        }

        canvas.Display();
    }

    return 0;
}