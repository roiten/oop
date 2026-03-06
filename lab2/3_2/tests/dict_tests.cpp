#include <catch2/catch_test_macros.hpp>
#include "../src/Dict.h"

#include <fstream>

//тесты на проверку регистронезависимости

TEST_CASE("FindTranslation возвращает перевод")
{
    Dictionary dict;
    dict["cat"] = {"кот", "кошка"};

    auto result = FindTranslation(dict, "cat");

    REQUIRE(result.size() == 2);
    REQUIRE(result.contains("кот"));
    REQUIRE(result.contains("кошка"));
}

TEST_CASE("FindTranslation возвращает пустой список если слова нет")
{
    Dictionary dict;
    dict["cat"] = {"кот"};

    auto result = FindTranslation(dict, "dog");

    REQUIRE(result.empty());
}

TEST_CASE("AddWordPair и двусторонняя связь")
{
    Dictionary dict;

    AddWordPair(dict, "cat", "кот, кошка");

    REQUIRE(dict["cat"].contains("кот"));
    REQUIRE(dict["cat"].contains("кошка"));

    REQUIRE(dict["кот"].contains("cat"));
    REQUIRE(dict["кошка"].contains("cat"));
}

TEST_CASE("AddWordPair переводы не дублируем")
{
    Dictionary dict;

    AddWordPair(dict, "cat", "кот");
    AddWordPair(dict, "cat", "кот");

    REQUIRE(dict["cat"].size() == 1);
    REQUIRE(dict["кот"].size() == 1);
}

TEST_CASE("SaveDictionary сохраняет словарь в файл")
{
    Dictionary dict;
    dict["cat"] = {"кот"};
    dict["пёс"] = {"dog"};

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
    dict["cat"] = {"кот"};

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