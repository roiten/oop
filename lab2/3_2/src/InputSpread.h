#pragma once
#include <algorithm>
#include <codecvt>
#include <cwctype>
#include <locale>
#include <string>

inline std::string Trim(const std::string& str)
{
	if (str.empty())
		return "";

	const size_t start = str.find_first_not_of(' ');
	if (start == std::string::npos)
		return "";

	const size_t end = str.find_last_not_of(' ');
	return str.substr(start, end - start + 1);
}

inline std::string NormalizeText(const std::string& input)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring wstr = converter.from_bytes(Trim(input));

	//transform
	std::ranges::transform(wstr, wstr.begin(), ::tolower);

	return converter.to_bytes(wstr);
}
