#pragma once
#include "IShape.h"
#include "Point.h"

#include <iomanip>
#include <sstream>
#include <string>

class CLineSegment : public IShape
{
public:
	CLineSegment(const CPoint& start, const CPoint& end, uint32_t outlineColor);

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;
	uint32_t GetOutlineColor() const override;

	CPoint GetStartPoint() const;
	CPoint GetEndPoint() const;

private:
	CPoint m_start, m_end;
	uint32_t m_outlineColor;
};