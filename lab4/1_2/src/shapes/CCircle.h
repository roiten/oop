#pragma once
#include "ISolidShape.h"
#include "Point.h"
#include <iomanip>
#include <sstream>
#include <string>

class CCircle : public ISolidShape
{
public:
	CCircle(const CPoint& center, double radius, uint32_t outlineColor, uint32_t fillColor);

	double GetArea() const final;
	double GetPerimeter() const final;
	std::string GetType() const final;
	uint32_t GetOutlineColor() const final;
	uint32_t GetFillColor() const final;

	CPoint GetCenter() const;
	double GetRadius() const;

private:
	void AppendProperties(std::ostream& output) const final;

	CPoint m_center{};
	double m_radius;
	uint32_t m_outlineColor, m_fillColor;
};