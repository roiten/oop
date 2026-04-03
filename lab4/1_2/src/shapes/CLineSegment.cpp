#include "CLineSegment.h"

CLineSegment::CLineSegment(const CPoint& start, const CPoint& end, uint32_t outlineColor)
	: m_start(start), m_end(end), m_outlineColor(outlineColor) {}

double CLineSegment::GetArea() const
{
	return 0.0;
}

double CLineSegment::GetPerimeter() const
{
	double dx = m_end.x - m_start.x;
	double dy = m_end.y - m_start.y;
	return std::sqrt(dx * dx + dy * dy);
}

std::string CLineSegment::ToString() const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2);
	oss << "LineSegment:\n  Start: (" << m_start.x << ", " << m_start.y << ")\n"
		<< "  End: (" << m_end.x << ", " << m_end.y << ")\n"
		<< "  Length: " << GetPerimeter() << "\n"
		<< "  Outline color: #" << std::hex << std::setw(6) << std::setfill('0') << m_outlineColor << std::dec;
	return oss.str();
}

uint32_t CLineSegment::GetOutlineColor() const
{
	return m_outlineColor;
}

CPoint CLineSegment::GetStartPoint() const
{
	return m_start;
}

CPoint CLineSegment::GetEndPoint() const
{
	return m_end;
}