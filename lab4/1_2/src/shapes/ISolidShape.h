#pragma once
#include "IShape.h"
#include "sstream"
#include <string>
#include <cstdint>

class ISolidShape : public IShape
{
public:
	virtual uint32_t GetFillColor() const = 0; //вынести final-ом
	virtual std::string GetType() const = 0;
	std::string ToString() const final
	{
		std::ostringstream s;
		s << GetType() << " ";
		AppendProperties(s);
		s << " area: " << GetArea();
		s << " perimeter: " << GetPerimeter();
		return s.str();
	}
private:
	virtual void AppendProperties(std::ostream&) const {}
};