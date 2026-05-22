#include "Dict.h"
#include "InputSpread.h"

#include <iostream>

void RunDictionary(Dictionary& dictionary, const std::string& fileName);
bool AskToSave();
std::string GetFileName(const int argc, char* argv[]);

int main(const int argc, char* argv[])
{
	std::locale::global(std::locale("ru_RU.utf8"));
	try
	{
		const std::string fileName = GetFileName(argc, argv);

		Dictionary dictionary = LoadDictionary(fileName);
		RunDictionary(dictionary, fileName);
	}
	catch (const std::exception& errorMessage)
	{
		std::cerr << errorMessage.what() << std::endl;
		return 1;
	}

	return 0;
}

std::string GetFileName(const int argc, char* argv[])
{
	if (argc != 2)
	{
		throw std::runtime_error("Укажите имя файла со словарём!");
	}
	return argv[1];
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
				SaveDictionary(fileName, dictionary);
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
				AddTranslation(dictionary, NormalizeText(input), translationLine);
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

bool AskToSave()
{
	std::cout << "Сохранить изменения? (y/n)\n";
	std::string answer;
	std::getline(std::cin, answer);

	answer = Trim(answer);
	return answer == "y" || answer == "Y";
}