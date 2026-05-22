#include "../src/ParseUrl.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Valid HTTP URL without port and document", "[ParseURL]")
{
    const std::string url = "http://example.com";
    UrlData expected{
        Protocol::HTTP,
        "example.com",
        80,
        ""
    };

    UrlData actual;
    REQUIRE(ParseURL(url, actual));
    REQUIRE(expected == actual);
}

TEST_CASE("Valid HTTPS URL with port and document", "[ParseURL]")
{
    const std::string url = "https://example.com:8080/index.html";
    UrlData expected{
        Protocol::HTTPS,
        "example.com",
        8080,
        "index.html"
    };

    UrlData actual;
    REQUIRE(ParseURL(url, actual));
    REQUIRE(expected == actual);
}

TEST_CASE("Valid FTP URL with document", "[ParseURL]")
{
    const std::string url = "ftp://ftp.example.com/files/file.txt";
    UrlData expected{
        Protocol::FTP,
        "ftp.example.com",
        21,
        "files/file.txt"
    };

    UrlData actual;
    REQUIRE(ParseURL(url, actual));
    REQUIRE(expected == actual);
}

TEST_CASE("Valid URL with mixed case protocol", "[ParseURL]")
{
    const std::string url = "HtTpS://example.com";
    UrlData expected{
        Protocol::HTTPS,
        "example.com",
        443,
        ""
    };

    UrlData actual;
    REQUIRE(ParseURL(url, actual));
    REQUIRE(expected == actual);
}

TEST_CASE("Invalid protocol", "[ParseURL][Negative]")
{
    const std::string url = "htp://example.com";
    UrlData data;
    REQUIRE_FALSE(ParseURL(url, data));
}

TEST_CASE("Missing protocol separator", "[ParseURL][Negative]")
{
    const std::string url = "http//example.com";
    UrlData data;
    REQUIRE_FALSE(ParseURL(url, data));
}

TEST_CASE("Invalid port number", "[ParseURL][Negative]")
{
    const std::string url = "http://example.com:0";
    UrlData data;
    REQUIRE_FALSE(ParseURL(url, data));
}

TEST_CASE("Port number out of range", "[ParseURL][Negative]")
{
    const std::string url = "http://example.com:65536";
    UrlData data;
    REQUIRE_FALSE(ParseURL(url, data));
}

TEST_CASE("Missing host", "[ParseURL][Negative]")
{
    const std::string url = "http://:8080";
    UrlData data;
    REQUIRE_FALSE(ParseURL(url, data));
}

TEST_CASE("Empty string", "[ParseURL][Negative]")
{
    const std::string url = "";
    UrlData data;
    REQUIRE_FALSE(ParseURL(url, data));
}

TEST_CASE("Valid URL with max port number", "[ParseURL]")
{
    const std::string url = "http://example.com:65535";
    UrlData expected{
        Protocol::HTTP,
        "example.com",
        65535,
        ""
    };

    UrlData actual;
    REQUIRE(ParseURL(url, actual));
    REQUIRE(expected == actual);
}

TEST_CASE("Valid URL with min port number", "[ParseURL]")
{
    const std::string url = "http://example.com:1";
    UrlData expected{
        Protocol::HTTP,
        "example.com",
        1,
        ""
    };

    UrlData actual;
    REQUIRE(ParseURL(url, actual));
    REQUIRE(expected == actual);
}

TEST_CASE("URL with empty document", "[ParseURL]")
{
    const std::string url = "http://example.com/";
    UrlData expected{
        Protocol::HTTP,
        "example.com",
        80,
        ""
    };

    UrlData actual;
    REQUIRE(ParseURL(url, actual));
    REQUIRE(expected == actual);
}

TEST_CASE("URL with only protocol and host", "[ParseURL]")
{
    const std::string url = "ftp://host";
    UrlData expected{
        Protocol::FTP,
        "host",
        21,
        ""
    };

    UrlData actual;
    REQUIRE(ParseURL(url, actual));
    REQUIRE(expected == actual);
}

TEST_CASE("URL with trailing slash after port", "[ParseURL]")
{
    const std::string url = "http://example.com:8080/";
    UrlData expected{
        Protocol::HTTP,
        "example.com",
        8080,
        ""
    };

    UrlData actual;
    REQUIRE(ParseURL(url, actual));
    REQUIRE(expected == actual);
}

TEST_CASE("Invalid port format", "[ParseURL][Negative]")
{
    const std::string url = "http://example.com:port";
    UrlData data;
    REQUIRE_FALSE(ParseURL(url, data));
}

//я могу создать url с с портом необычным для этого протоколоа?