#include "FindProperties.h"

std::shared_ptr<IShape> FindShapeWithMaxArea(const std::vector<std::shared_ptr<IShape>>& shapes)
{
	if (shapes.empty())
		return nullptr;
	return *std::ranges::max_element(shapes,
		[](const auto& shape1, const auto& shape2) { return shape1->GetArea() < shape2->GetArea(); });
}

std::shared_ptr<IShape> FindShapeWithMinPerimeter(const std::vector<std::shared_ptr<IShape>>& shapes)
{
	if (shapes.empty())
		return nullptr;
	return *std::ranges::min_element(shapes,
		[](const auto& s1, const auto& s2) { return s1->GetPerimeter() < s2->GetPerimeter(); });
}
