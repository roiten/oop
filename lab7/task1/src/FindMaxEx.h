#ifndef LABS_FINDMAXEX_H
#define LABS_FINDMAXEX_H
#include <vector>

template <typename T, typename Less>
bool FindMaxEx(std::vector<T> const& arr, T& maxValue, Less const& less)
{
	if (arr.empty()) return false;

	size_t maxIndex = 0;
	for (size_t i = 1; i < arr.size(); ++i)
	{
		if (less(arr[maxIndex], arr[i]))
		{
			maxIndex = i;
		}
	}

	maxValue = arr[maxIndex];
	return true;
}

#endif
