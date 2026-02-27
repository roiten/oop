#pragma once
#include <ostream>
#include <string>

void HtmlDecodeAndPrint(std::istream& input, std::ostream& output);
std::string HtmlDecode(const std::string& text);