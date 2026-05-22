#ifndef LABS_SORT2_H
#define LABS_SORT2_H

#include <cstring>
#include <iostream>
#include <string>

template <typename T>
void Sort2(T& first, T& second)
{
	if (second < first)
	{
		std::swap(first, second);
	}
}

template <>
inline void Sort2<const char*>(const char*& first, const char*& second)
{
	int minLen = std::min(std::strlen(first), std::strlen(second));
	for (int i = 0; i < minLen; i++)
	{
		if (first[i] > second[i])
		{
			std::swap(first, second);
			return;
		}

		if (i == minLen - 1 && first[i] == second[i] && std::strlen(first) > std::strlen(second))
		{
			std::swap(first, second);
			return;
		}
	}
}


#endif
