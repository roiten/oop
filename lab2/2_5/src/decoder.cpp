#include "decoder.h"
#include <unordered_map>
#include <string_view>
#include <iostream>

const std::unordered_map<std::string_view, char> HTML_DECODE_MAP = {
	{ "&quot;", '"' },
	{ "&apos;", '\'' },
	{ "&lt;",   '<' },
	{ "&gt;",   '>' },
	{ "&amp;",  '&' }
};

void HtmlDecodeAndPrint(std::istream& input, std::ostream& output)
{
	std::string line;
	while (std::getline(input, line))
	{
		output << HtmlDecode(line) << '\n';
	}
}

std::string HtmlDecode(const std::string& text)
{
	std::string result;
	result.reserve(text.size());

	for (size_t i = 0; i < text.size(); ++i)
	{
		if (text[i] != '&')
		{
			result += text[i];
			continue;
		}

		size_t semicolonPos = text.find(';', i);
		if (semicolonPos == std::string::npos)
		{
			result += text[i];
			continue;
		}

		std::string_view entity(&text[i], semicolonPos - i + 1);

		if (auto it = HTML_DECODE_MAP.find(entity); it != HTML_DECODE_MAP.end())
		{
			result += it->second;
			i = semicolonPos;
		}
		else
		{
			result += text[i];
		}
	}

	return result;
}