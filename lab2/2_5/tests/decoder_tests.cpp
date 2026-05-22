#include "../src/Decoder.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>

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

TEST_CASE("тест с кошкой")
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
	REQUIRE(HtmlDecode("amp;") == "amp;");

}

TEST_CASE("вложенные html сущности")
{
    REQUIRE(HtmlDecode("&amp;quot;") == "&quot;");
    REQUIRE(HtmlDecode("&amp;amp;") == "&amp;");
}

TEST_CASE("Пустой вывод при пустом вводе")
{
    REQUIRE(HtmlDecode("").empty());
}