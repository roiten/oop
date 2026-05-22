#pragma once
#include "ICanvasDrawable.h"
#include "IShape.h"
#include "sstream"
#include <string>

class ISolidShape : public IShape {
private:
	virtual void AppendProperties(std::ostream&) const {}

public:
	virtual uint32_t GetFillColor() const = 0;
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
};