#pragma once
#include "ISolidShape.h"
#include "Point.h"
#include <string>
#include <cstdint>

class CTriangle : public ISolidShape
{
public:
	CTriangle(const CPoint& v1, const CPoint& v2, const CPoint& v3, uint32_t outlineColor, uint32_t fillColor);

	double GetArea() const final;
	double GetPerimeter() const final;
	uint32_t GetOutlineColor() const final;
	uint32_t GetFillColor() const final;
	std::string GetType() const final;

	CPoint GetVertex1() const;
	CPoint GetVertex2() const;
	CPoint GetVertex3() const;

private:
	double GetSideLength(CPoint p1, CPoint p2) const;
	void AppendProperties(std::ostream& output) const final;

	CPoint m_v1, m_v2, m_v3;
	uint32_t m_outlineColor, m_fillColor;
};