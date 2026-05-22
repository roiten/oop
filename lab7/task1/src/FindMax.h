#ifndef LABS_FINDMAX_H
#define LABS_FINDMAX_H
#include <vector>

template <typename T>
bool FindMax(std::vector<T> const& arr, T& maxValue)
{
	if (arr.empty()) return false;

	size_t maxIndex = 0;

	for (size_t i = 1; i < arr.size(); ++i)
	{
		if (arr[i] > arr[maxIndex])
		{
			maxIndex = i;
		}
	}

	maxValue = arr[maxIndex];

	// maxValue = *std::max_element(arr.begin(), arr.end());
	return true;
}

template <>
inline bool FindMax<const char*>(std::vector<const char*> const& arr, const char* &maxValue)
{
	if (arr.empty()) return false;
	size_t maxIndex = 0;

	for (size_t i = 1; i < arr.size(); ++i)
	{
		if (strcmp(arr[i], arr[maxIndex]) > 0)
		{
			maxIndex = i;
		}
	}

	maxValue = arr[maxIndex];

	return true;
}

#endif
