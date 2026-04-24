#include "CCircle.h"
#include <numbers>

CCircle::CCircle(const CPoint& center, double radius, uint32_t outlineColor, uint32_t fillColor)
{
	if (radius <= 0)
	{
		throw std::invalid_argument("radius must be greater than zero");
	}
	m_center = center;
	m_radius = radius;
	m_outlineColor = outlineColor;
	m_fillColor = fillColor;
}

double CCircle::GetArea() const
{
	return std::numbers::pi * m_radius * m_radius;
}

double CCircle::GetPerimeter() const
{
	return 2 * std::numbers::pi * m_radius;
}

std::string CCircle::GetType() const
{
	return "Circle";
}

void CCircle::AppendProperties(std::ostream& output) const
{
	output << std::fixed << std::setprecision(2);
	output << "Center: (" << m_center.x << ", " << m_center.y << ")\n"
		   << "  Radius: " << m_radius << "\n"
		   << "  Outline color: #" << std::hex << std::setw(6) << std::setfill('0') << m_outlineColor << "\n"
		   << "  Fill color: #" << std::setw(6) << std::setfill('0') << m_fillColor << std::endl;
}

uint32_t CCircle::GetOutlineColor() const
{
	return m_outlineColor;
}

uint32_t CCircle::GetFillColor() const
{
	return m_fillColor;
}

CPoint CCircle::GetCenter() const
{
	return m_center;
}

double CCircle::GetRadius() const
{
	return m_radius;
}