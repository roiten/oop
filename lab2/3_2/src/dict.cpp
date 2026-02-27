#include "dict.h"
#include <algorithm>
#include <codecvt>
#include <cwctype>
#include <fstream>
#include <iostream>
#include <locale>

std::string Trim(const std::string& str)
{
    if (str.empty())
        return "";

    size_t start = str.find_first_not_of(' ');
    if (start == std::string::npos)
        return "";

    size_t end = str.find_last_not_of(' ');
    return str.substr(start, end - start + 1);
}

std::string NormalizeText(const std::string& input)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(Trim(input));

    for (wchar_t& ch : wstr)
    {
        ch = std::towlower(ch);
    }

    return converter.to_bytes(wstr);
}

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
        throw std::runtime_error("Отсутствует перевод в строке (" + line + ")");
    }

    return result;
}

std::pair<std::string, ListWords> ParseDictionaryLine(const std::string& line)
{
    size_t pos = line.find(" - ");
    if (pos == std::string::npos)
    {
        throw std::runtime_error("Некорректный формат строки (" + line + ")");
    }

    std::string word = NormalizeText(line.substr(0, pos));
    std::string translations = line.substr(pos + 3);

    return { word, SplitTranslations(translations) };
}

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

void AddWordPair(Dictionary& dictionary,
                        const std::string& word,
                        const std::string& translationLine)
{
    ListWords translations = SplitTranslations(translationLine);

    for (const auto& translation : translations)
    {
        dictionary[word].insert(translation);
        dictionary[translation].insert(word);
    }
}

bool AskToSave()
{
    std::cout << "Сохранить изменения? (y/n)\n";
    std::string answer;
    std::getline(std::cin, answer);

    answer = Trim(answer);
    return answer == "y" || answer == "Y";
}

void WriteDictionary(const std::string& fileName, const Dictionary& dictionary)
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
            first = false; // FIRST!
        }

        file << std::endl;
    }
}

std::string GetFileName(int argc, char* argv[])
{
    if (argc != 2)
    {
        throw std::exception("Укажите имя файла со словарём!");
    }
    return argv[1];
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

void RunDictionary(Dictionary& dictionary, const std::string& fileName)
{
    bool modified = false;

    while (true)
    {
        std::cout << ">";
        std::string input;
        std::getline(std::cin, input);

        if (input == "...")
        {
            if (modified && AskToSave())
            {
                WriteDictionary(fileName, dictionary);
                std::cout << "Изменения сохранены.\n";
            }
            else if (modified)
            {
                std::cout << "Изменения не сохранены.\n";
            }
            break;
        }

        ListWords translation = FindTranslation(dictionary, input);

        if (translation.empty())
        {
            std::cout << "Неизвестное слово \"" << input
                      << "\". Введите перевод или пустую строку.\n";

            std::string translationLine;
            std::getline(std::cin, translationLine);
            translationLine = Trim(translationLine);

            if (!translationLine.empty())
            {
                AddWordPair(dictionary, NormalizeText(input), translationLine);
                std::cout << "Слово сохранено.\n";
                modified = true;
            }
            else
            {
                std::cout << "Слово проигнорировано.\n";
            }
        }
        else
        {
            PrintTranslations(translation);
        }
    }
}