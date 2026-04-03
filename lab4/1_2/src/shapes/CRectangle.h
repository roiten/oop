#pragma once
#include "ISolidShape.h"
#include "Point.h"
#include <string>

class CRectangle : public ISolidShape
{
public:
	CRectangle(const CPoint& leftTop, double width, double height, uint32_t outlineColor, uint32_t fillColor);

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;
	uint32_t GetOutlineColor() const override;
	uint32_t GetFillColor() const override;

	CPoint GetLeftTop() const;
	double GetWidth() const;
	double GetHeight() const;

private:
	CPoint m_leftTop;
	double m_width, m_height;
	uint32_t m_outlineColor, m_fillColor;
};