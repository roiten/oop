#include "CCircle.h"
#include <numbers>

CCircle::CCircle(const CPoint& center, double radius, uint32_t outlineColor, uint32_t fillColor)
	: m_center(center), m_radius(radius), m_outlineColor(outlineColor), m_fillColor(fillColor) {}

double CCircle::GetArea() const
{
	return std::numbers::pi * m_radius * m_radius;
}

double CCircle::GetPerimeter() const
{
	return 2 * std::numbers::pi * m_radius;
}

std::string CCircle::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2);
	oss << "Circle:\n  Center: (" << m_center.x << ", " << m_center.y << ")\n"
		<< "  Radius: " << m_radius << "\n"
		<< "  Area: " << GetArea() << "\n  Perimeter: " << GetPerimeter() << "\n"
		<< "  Outline color: #" << std::hex << std::setw(6) << std::setfill('0') << m_outlineColor << "\n"
		<< "  Fill color: #" << std::setw(6) << std::setfill('0') << m_fillColor << std::dec;
	return oss.str();
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