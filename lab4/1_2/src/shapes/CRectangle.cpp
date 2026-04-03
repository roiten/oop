#include "CRectangle.h"
#include <iomanip>
#include <sstream>

CRectangle::CRectangle(const CPoint& leftTop, double width, double height, uint32_t outlineColor, uint32_t fillColor)
	: m_leftTop(leftTop), m_width(width), m_height(height), m_outlineColor(outlineColor), m_fillColor(fillColor) {}

double CRectangle::GetArea() const
{
	return m_width * m_height;
}

double CRectangle::GetPerimeter() const
{
	return 2 * (m_width + m_height);
}

std::string CRectangle::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2);
	oss << "Rectangle:\n  Left top: (" << m_leftTop.x << ", " << m_leftTop.y << ")\n"
		<< "  Width: " << m_width << "\n  Height: " << m_height << "\n"
		<< "  Area: " << GetArea() << "\n  Perimeter: " << GetPerimeter() << "\n"
		<< "  Outline color: #" << std::hex << std::setw(6) << std::setfill('0') << m_outlineColor << "\n"
		<< "  Fill color: #" << std::setw(6) << std::setfill('0') << m_fillColor << std::dec;
	return oss.str();
}

uint32_t CRectangle::GetOutlineColor() const
{
	return m_outlineColor;
}

uint32_t CRectangle::GetFillColor() const
{
	return m_fillColor;
}

CPoint CRectangle::GetLeftTop() const
{
	return m_leftTop;
}

double CRectangle::GetWidth() const
{
	return m_width;
}

double CRectangle::GetHeight() const
{
	return m_height;
}