#include "../src/Dict.h"
#include <catch2/catch_test_macros.hpp>

#include <fstream>

// тесты на проверку регистронезависимости

TEST_CASE("тесты на проверку регистронезависимости")
{
	SECTION("поиск по ключу в нижнем регистре")
	{
		Dictionary dict;
		AddTranslation(dict, "CAT", "кОт");

		auto result = FindTranslation(dict, "cat");

		REQUIRE(result.size() == 1);
		REQUIRE(result.contains("кот"));
	}

	SECTION("поиск по ключу в верхнем регистре")
	{
		Dictionary dict;
		AddTranslation(dict, "cat", "кошка");

		auto result = FindTranslation(dict, "КОШКА");

		REQUIRE(result.size() == 1);
		REQUIRE(result.contains("кошка"));
	}

	SECTION("Поиск по ключу со смешанным регистром")
	{
		Dictionary dict;
		AddTranslation(dict, "DoG", "собака, пёс");
		auto result = FindTranslation(dict, "Dog");

		REQUIRE(result.size() == 2);
		REQUIRE(result.contains("собака"));
		REQUIRE(result.contains("пёс"));
	}
}

TEST_CASE("FindTranslation возвращает перевод")
{
	Dictionary dict;
	dict["cat"] = { "кот", "кошка" };

	auto result = FindTranslation(dict, "cat");

	REQUIRE(result.size() == 2);
	REQUIRE(result.contains("кот"));
	REQUIRE(result.contains("кошка"));
}

TEST_CASE("FindTranslation возвращает пустой список если слова нет")
{
	Dictionary dict;
	dict["cat"] = { "кот" };

	auto result = FindTranslation(dict, "dog");

	REQUIRE(result.empty());
}

TEST_CASE("AddWordPair и двусторонняя связь")
{
	Dictionary dict;

	AddTranslation(dict, "cat", "кот, кошка");

	REQUIRE(dict["cat"].contains("кот"));
	REQUIRE(dict["cat"].contains("кошка"));

	REQUIRE(dict["кот"].contains("cat"));
	REQUIRE(dict["кошка"].contains("cat"));
}

TEST_CASE("AddTranslation переводы не дублируем")
{
	Dictionary dict;

	AddTranslation(dict, "cat", "кот");
	AddTranslation(dict, "cat", "кот");

	REQUIRE(dict["cat"].size() == 1);
	REQUIRE(dict["кот"].size() == 1);
}

TEST_CASE("SaveDictionary сохраняет словарь в файл")
{
	Dictionary dict;
	dict["cat"] = { "кот" };
	dict["пёс"] = { "dog" };

	const std::string fileName = "test_save.txt";

	SaveDictionary(fileName, dict);

	std::ifstream file(fileName);
	REQUIRE(file.is_open());

	std::string content((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	REQUIRE(content.find("cat - кот") != std::string::npos);
	REQUIRE(content.find("пёс - dog") != std::string::npos);
}

TEST_CASE("выбрасываем исключение если файл нельзя открыть")
{
	Dictionary dict;
	dict["cat"] = { "кот" };

	REQUIRE_THROWS(SaveDictionary("/invalid_path/test.txt", dict));
}

TEST_CASE("LoadDictionary загружает словарь из файла")
{
	const std::string fileName = "test_load.txt";

	{
		std::ofstream file(fileName);
		file << "cat - кот, кошка\n";
		file << "dog - собака\n";
	}

	Dictionary dict = LoadDictionary(fileName);

	REQUIRE(dict.contains("cat"));
	REQUIRE(dict.contains("dog"));

	REQUIRE(dict["cat"].contains("кот"));
	REQUIRE(dict["cat"].contains("кошка"));
	REQUIRE(dict["dog"].contains("собака"));
}

TEST_CASE("LoadDictionary возвращает пустой словарь если файл не найден")
{
	Dictionary dict = LoadDictionary("nonexist_file.txt");
	REQUIRE(dict.empty());
}