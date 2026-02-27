#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "../src/decoder.h"

TEST_CASE("пустая строка в HtmlDecode")
{
    REQUIRE(HtmlDecode("").empty());
}

TEST_CASE("строка без html-сущностей в HtmlDecode")
{
    REQUIRE(HtmlDecode("Hello World") == "Hello World");
    REQUIRE(HtmlDecode("12345Q") == "12345Q");
}

TEST_CASE("сопоставление html-сущностей")
{
    REQUIRE(HtmlDecode("&quot;") == "\"");
    REQUIRE(HtmlDecode("&apos;") == "'");
    REQUIRE(HtmlDecode("&lt;") == "<");
    REQUIRE(HtmlDecode("&gt;") == ">");
    REQUIRE(HtmlDecode("&amp;") == "&");
}

TEST_CASE("HtmlDecode и тест из задания")
{
    std::string input = "Cat &lt;says&gt; &quot;Meow&quot;. M&amp;M&apos;s";
    std::string expected = "Cat <says> \"Meow\". M&M's";
    REQUIRE(HtmlDecode(input) == expected);
}

TEST_CASE("битые сущности")
{
    REQUIRE(HtmlDecode("&amp") == "&amp");
    REQUIRE(HtmlDecode("Hello &lt World") == "Hello &lt World");
    REQUIRE(HtmlDecode("A & B") == "A & B");
}

TEST_CASE("вложенные html сущности")
{
    REQUIRE(HtmlDecode("&amp;quot;") == "&quot;");
    REQUIRE(HtmlDecode("&amp;amp;") == "&amp;");
}

TEST_CASE("дешифровка и печать строки")
{
    std::stringstream input("Cat &lt;says&gt; &quot;Meow&quot;");
    std::stringstream output;
    HtmlDecodeAndPrint(input, output);
    REQUIRE(output.str() == "Cat <says> \"Meow\"\n");
}

TEST_CASE("дешифровка и печать нескольких строк")
{
    std::stringstream input;
    input << "First &lt;line&gt;\n";
    input << "Second &quot;line&quot;\n";
    input << "M&amp;M&apos;s\n";
    
    std::stringstream output;
    HtmlDecodeAndPrint(input, output);
    
    std::string expected = "First <line>\n";
    expected += "Second \"line\"\n";
    expected += "M&M's\n";
    REQUIRE(output.str() == expected);
}

TEST_CASE("HtmlDecodeAndPrint: пустой ввод")
{
    std::stringstream input;
    std::stringstream output;
    HtmlDecodeAndPrint(input, output);
    REQUIRE(output.str() == "");
}