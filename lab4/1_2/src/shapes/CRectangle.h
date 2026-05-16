#pragma once
#include "ISolidShape.h"
#include "Point.h"
#include <string>
#include <cstdint>

class CRectangle : public ISolidShape
{
public:
	CRectangle(const CPoint& leftTop, double width, double height, uint32_t outlineColor, uint32_t fillColor);

	double GetArea() const final;
	double GetPerimeter() const final;
	std::string GetType() const final;
	uint32_t GetOutlineColor() const final;
	uint32_t GetFillColor() const final;

	CPoint GetLeftTop() const;
	double GetWidth() const;
	double GetHeight() const;

private:
	void AppendProperties(std::ostream& output) const final;

	CPoint m_leftTop;
	double m_width, m_height;
	uint32_t m_outlineColor, m_fillColor;
};