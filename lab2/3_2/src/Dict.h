#pragma once
#include <set>
#include <string>
#include <unordered_map>

using ListWords = std::set<std::string>;
using Dictionary = std::unordered_map<std::string, ListWords>;

Dictionary LoadDictionary(const std::string& fileName);
ListWords FindTranslation(const Dictionary& dictionary, const std::string& word);

void SaveDictionary(const std::string& fileName, const Dictionary& dictionary);

void AddWordPair(Dictionary& dictionary, //AddTranslation
	const std::string& word,
	const std::string& translationLine);

void PrintTranslations(const ListWords& translations);