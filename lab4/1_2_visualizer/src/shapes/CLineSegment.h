#pragma once
#include "IShape.h"
#include "Point.h"
#include "canvas/ICanvas.h"

#include <iomanip>
#include <sstream>
#include <string>

class CLineSegment : public IShape
{
public:
	CLineSegment(const CPoint& start, const CPoint& end, uint32_t outlineColor);

	double GetArea() const final;
	double GetPerimeter() const final;
	std::string ToString() const final;
	uint32_t GetOutlineColor() const final;
	void Draw(ICanvas& canvas) const final;

	CPoint GetStartPoint() const;
	CPoint GetEndPoint() const;

private:
	CPoint m_start, m_end;
	uint32_t m_outlineColor;
};