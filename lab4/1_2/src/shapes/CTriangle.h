#pragma once
#include "ISolidShape.h"
#include "Point.h"
#include <string>

class CTriangle : public ISolidShape
{
public:
	CTriangle(const CPoint& v1, const CPoint& v2, const CPoint& v3, uint32_t outlineColor, uint32_t fillColor);

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;
	uint32_t GetOutlineColor() const override;
	uint32_t GetFillColor() const override;

	CPoint GetVertex1() const;
	CPoint GetVertex2() const;
	CPoint GetVertex3() const;

private:
	double GetSideLength(CPoint p1, CPoint p2) const;

	CPoint m_v1, m_v2, m_v3;
	uint32_t m_outlineColor, m_fillColor;
};