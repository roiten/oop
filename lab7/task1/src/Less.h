#ifndef LABS_SORT2_H
#define LABS_SORT2_H

#include <cstring>
#include <iostream>
#include <string>

template <typename T>
bool Less(const T& first, const T& second)
{
	if (second <= first)
	{
		return true;
	}
	return false;
}

template <>
inline bool Less<const char*>(const char*& first, const char*& second)
{
	int minLen = std::min(std::strlen(first), std::strlen(second));
	for (int i = 0; i < minLen; i++)
	{
		if (first[i] > second[i])
		{
			return false;
		}

		if (i == minLen - 1 && first[i] == second[i] && std::strlen(first) > std::strlen(second))
		{
			return false;
		}
	}
	return true;
}


#endif
