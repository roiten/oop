#ifndef LABS_SPORTSMEN_H
#define LABS_SPORTSMEN_H

#include <iostream>
#include <string>

class Sportsmen
{
private:
	std::string fullName;
	int height{};
	int weight{};

public:
	Sportsmen() {}

	Sportsmen(const std::string& name, int h, int w)
		: fullName(name)
		, height(h)
		, weight(w)
	{
	}

	std::string getName() const { return fullName; }
	int getHeight() const { return height; }
	int getWeight() const { return weight; }

	friend std::ostream& operator<<(std::ostream& os, const Sportsmen& a)
	{
		os << a.fullName << " рост: " << a.height << " см вес: " << a.weight << " кг)";
		return os;
	}
};

struct CompareByHeight
{
	bool operator()(const Sportsmen& a, const Sportsmen& b) const
	{
		return a.getHeight() < b.getHeight();
	}
};

struct CompareByWeight
{
	bool operator()(const Sportsmen& a, const Sportsmen& b) const
	{
		return a.getWeight() < b.getWeight();
	}
};

#endif
