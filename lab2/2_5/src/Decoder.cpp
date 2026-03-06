#include "Decoder.h"
#include <iostream>
#include <string_view>
#include <unordered_map>

const std::unordered_map<std::string_view, char> HTML_DECODE_MAP = {
	{ "&quot;", '"' },
	{ "&apos;", '\'' },
	{ "&lt;",   '<' },
	{ "&gt;",   '>' },
	{ "&amp;",  '&' }
};

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

		if (auto founded = HTML_DECODE_MAP.find(entity); founded != HTML_DECODE_MAP.end())
		{
			result += founded->second;
			i = semicolonPos;
		}
		else
		{
			result += text[i];
		}
	}

	return result;
}