#include "CRectangle.h"
#include <iomanip>
#include <sstream>
#include <vector>
#include "canvas/ICanvas.h"

CRectangle::CRectangle(const CPoint& leftTop, double width, double height, uint32_t outlineColor, uint32_t fillColor)
{
	if (width < 0 || height < 0)
	{
		throw std::invalid_argument("rect properties can't be negative. Build your rect from the left top point. It can be negative");
	}
	m_leftTop = leftTop;
	m_width = width;
	m_height = height;
	m_outlineColor = outlineColor;
	m_fillColor = fillColor;
}

double CRectangle::GetArea() const
{
	return m_width * m_height;
}

double CRectangle::GetPerimeter() const
{
	return 2 * (m_width + m_height);
}

std::string CRectangle::GetType() const
{
	return "Rectangle";
}

void CRectangle::AppendProperties(std::ostream& output) const {
	output << std::fixed << std::setprecision(2);
	output << "Left top: (" << m_leftTop.x << ", " << m_leftTop.y << ")\n"
		   << "Width: " << m_width << "\n"
		   << "Height: " << m_height << "\n"
		   << "Outline color: #" << std::hex << std::setw(6) << std::setfill('0') << m_outlineColor << "\n"
		   << "Fill color: #" << std::setw(6) << std::setfill('0') << m_fillColor << std::dec;
}

void CRectangle::Draw(ICanvas& canvas) const
{
	std::vector<CPoint> points = {
		m_leftTop,
		{m_leftTop.x + m_width, m_leftTop.y},
		{m_leftTop.x + m_width, m_leftTop.y + m_height},
		{m_leftTop.x, m_leftTop.y + m_height}
	};
	canvas.FillPolygon(points, m_fillColor);

	canvas.DrawLine(points[0], points[1], m_outlineColor);
	canvas.DrawLine(points[1], points[2], m_outlineColor);
	canvas.DrawLine(points[2], points[3], m_outlineColor);
	canvas.DrawLine(points[3], points[0], m_outlineColor);
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