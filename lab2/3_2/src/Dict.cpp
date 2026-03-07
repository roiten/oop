#include "dict.h"

#include "InputSpread.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <locale>

ListWords SplitTranslations(const std::string& line)
{
	ListWords result;
	size_t start = 0;
	size_t end = line.find(", ");

	while (end != std::string::npos)
	{
		std::string word = Trim(line.substr(start, end - start));
		if (!word.empty())
			result.insert(word);

		start = end + 2;
		end = line.find(", ", start);
	}

	std::string last = Trim(line.substr(start));
	if (!last.empty())
		result.insert(last);

	if (result.empty())
	{
		throw std::exception("Отсутствует перевод");
	}

	return result;
}

std::pair<std::string, ListWords> ParseDictionaryLine(const std::string& line)
{
	size_t pos = line.find(" - ");
	if (pos == std::string::npos)
	{
		throw std::exception("Некорректный формат строки");
	}

	std::string word = NormalizeText(line.substr(0, pos));
	std::string translations = line.substr(pos + 3);

	return { word, SplitTranslations(translations) };
}

// реализуемые функции

ListWords FindTranslation(const Dictionary& dictionary, const std::string& word)
{
	std::string normalized = NormalizeText(word);

	auto it = dictionary.find(normalized);
	if (it != dictionary.end())
		return it->second;

	return {};
}

void PrintTranslations(const ListWords& translations)
{
	for (const auto& word : translations)
	{
		std::cout << word << std::endl;
	}
}

void AddTranslation(Dictionary& dictionary,
	const std::string& word,
	const std::string& translationLine)
{
	ListWords translations = SplitTranslations(translationLine);
	for (const auto& translation : translations)
	{
		dictionary[word].insert(NormalizeText(translation));
		dictionary[NormalizeText(translation)].insert(word);
	}
}

void SaveDictionary(const std::string& fileName, const Dictionary& dictionary)
{
	std::ofstream file(fileName);
	if (!file.is_open())
		throw std::exception("Не удалось открыть файл для записи");

	for (const auto& [word, translations] : dictionary)
	{
		file << word << " - ";

		bool first = true;
		for (const auto& translation : translations)
		{
			if (!first)
				file << ", ";

			file << translation;
			first = false;
		}

		file << std::endl;
	}
}

Dictionary LoadDictionary(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
		return {};

	Dictionary dictionary;
	std::string line;

	while (std::getline(file, line))
	{
		auto [word, translations] = ParseDictionaryLine(line);
		dictionary[word] = translations;
	}

	return dictionary;
}