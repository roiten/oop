#include "CCanvas.h"

CCanvas::CCanvas(sf::RenderWindow& window)
    : m_window(window)
{
}

sf::Color CCanvas::ConvertColor(uint32_t color) const
{
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    uint8_t a = (color >> 24) & 0xFF;

    if (a == 0) a = 255;

    return {r, g, b, a};
}

void CCanvas::DrawLine(const CPoint& from, const CPoint& to, uint32_t lineColor)
{
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(static_cast<float>(from.x), static_cast<float>(from.y)), ConvertColor(lineColor)),
        sf::Vertex(sf::Vector2f(static_cast<float>(to.x), static_cast<float>(to.y)), ConvertColor(lineColor))
    };
    m_window.draw(line, 2, sf::Lines);
}

void CCanvas::FillPolygon(const std::vector<CPoint>& points, uint32_t fillColor)
{
    if (points.size() < 3) return;

    sf::ConvexShape polygon;
    polygon.setPointCount(points.size());
    polygon.setFillColor(ConvertColor(fillColor));

	std::size_t index = 0;
	for (const auto& point : points)
	{
		polygon.setPoint(index++, sf::Vector2f(static_cast<float>(point.x), static_cast<float>(point.y)));
	}

    m_window.draw(polygon);
}

void CCanvas::DrawCircle(const CPoint& center, double radius, uint32_t lineColor)
{
    sf::CircleShape circle(static_cast<float>(radius));
    circle.setPosition(static_cast<float>(center.x - radius), static_cast<float>(center.y - radius));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(ConvertColor(lineColor));
    circle.setOutlineThickness(1.0f);
    m_window.draw(circle);
}

void CCanvas::FillCircle(const CPoint& center, double radius, uint32_t fillColor)
{
    sf::CircleShape circle(static_cast<float>(radius));
    circle.setPosition(static_cast<float>(center.x - radius), static_cast<float>(center.y - radius));
    circle.setFillColor(ConvertColor(fillColor));
    circle.setOutlineThickness(0);
    m_window.draw(circle);
}

void CCanvas::Clear(uint32_t backgroundColor)
{
    m_window.clear(ConvertColor(backgroundColor));
}

void CCanvas::Display()
{
    m_window.display();
}