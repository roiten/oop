#include "CTriangle.h"
#include <iomanip>
#include <sstream>
#include <cmath>
#include <cstdlib>

CTriangle::CTriangle(const CPoint& v1, const CPoint& v2, const CPoint& v3, uint32_t outlineColor, uint32_t fillColor)
	: m_v1(v1), m_v2(v2), m_v3(v3), m_outlineColor(outlineColor), m_fillColor(fillColor) {}

double CTriangle::GetArea() const
{
	return std::abs((m_v2.x - m_v1.x) * (m_v3.y - m_v1.y) - (m_v3.x - m_v1.x) * (m_v2.y - m_v1.y)) / 2.0;
}

double CTriangle::GetPerimeter() const
{
	return GetSideLength(m_v1, m_v2) + GetSideLength(m_v2, m_v3) + GetSideLength(m_v3, m_v1);
}

std::string CTriangle::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2);
	oss << "Triangle:\n  Vertex1: (" << m_v1.x << ", " << m_v1.y << ")\n"
		<< "  Vertex2: (" << m_v2.x << ", " << m_v2.y << ")\n"
		<< "  Vertex3: (" << m_v3.x << ", " << m_v3.y << ")\n"
		<< "  Area: " << GetArea() << "\n  Perimeter: " << GetPerimeter() << "\n"
		<< "  Outline color: #" << std::hex << std::setw(6) << std::setfill('0') << m_outlineColor << "\n"
		<< "  Fill color: #" << std::setw(6) << std::setfill('0') << m_fillColor << std::dec;
	return oss.str();
}

uint32_t CTriangle::GetOutlineColor() const
{
	return m_outlineColor;
}

uint32_t CTriangle::GetFillColor() const
{
	return m_fillColor;
}

CPoint CTriangle::GetVertex1() const
{
	return m_v1;
}

CPoint CTriangle::GetVertex2() const
{
	return m_v2;
}

CPoint CTriangle::GetVertex3() const
{
	return m_v3;
}

double CTriangle::GetSideLength(CPoint p1, CPoint p2) const
{
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	return std::sqrt(dx * dx + dy * dy);
}