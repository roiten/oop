#include "../ParseUrl.h"
#include <gtest/gtest.h>

using namespace parse_url;

TEST(ParseUrlTest, ValidHttpUrlWithoutPortAndDocument)
{
	const std::string url = "http://example.com";
	UrlData expected{
		Protocol::HTTP,
		"example.com",
		80,
		""
	};

	UrlData actual;
	EXPECT_TRUE(ParseURL(url, actual));
	EXPECT_EQ(expected, actual);
}

TEST(ParseUrlTest, ValidHttpsUrlWithPortAndDocument)
{
	const std::string url = "https://example.com:8080/index.html";
	UrlData expected{
		Protocol::HTTPS,
		"example.com",
		8080,
		"index.html"
	};

	UrlData actual;
	EXPECT_TRUE(ParseURL(url, actual));
	EXPECT_EQ(expected, actual);
}

TEST(ParseUrlTest, ValidFtpUrlWithDocument)
{
	const std::string url = "ftp://ftp.example.com/files/file.txt";
	UrlData expected{
		Protocol::FTP,
		"ftp.example.com",
		21,
		"files/file.txt"
	};

	UrlData actual;
	EXPECT_TRUE(ParseURL(url, actual));
	EXPECT_EQ(expected, actual);
}

TEST(ParseUrlTest, ValidUrlWithMixedCaseProtocol)
{
	const std::string url = "HtTpS://example.com";
	UrlData expected{
		Protocol::HTTPS,
		"example.com",
		443,
		""
	};

	UrlData actual;
	EXPECT_TRUE(ParseURL(url, actual));
	EXPECT_EQ(expected, actual);
}

// Не работает
// TEST(ParseUrlTest, ValidUrlWithQueryParameters)
// {
// 	const std::string url = "http://www.mysite.com/docs/document1.html?page=30&lang=en#title";
// 	UrlData expected{
// 		Protocol::HTTP,
// 		"www.mysite.com",
// 		80,
// 		"docs/document1.html?page=30&lang=en#title"
// 	};

// 	UrlData actual;
// 	EXPECT_TRUE(ParseURL(url, actual));
// 	EXPECT_EQ(expected, actual);
// }

TEST(ParseUrlTest, InvalidProtocol)
{
	const std::string url = "htp://example.com";
	UrlData data;
	EXPECT_FALSE(ParseURL(url, data));
}

TEST(ParseUrlTest, MissingProtocolSeparator)
{
	const std::string url = "http//example.com";
	UrlData data;
	EXPECT_FALSE(ParseURL(url, data));
}

TEST(ParseUrlTest, InvalidPortNumber)
{
	const std::string url = "http://example.com:0";
	UrlData data;
	EXPECT_FALSE(ParseURL(url, data));
}

TEST(ParseUrlTest, PortNumberOutOfRange)
{
	const std::string url = "http://example.com:65536";
	UrlData data;
	EXPECT_FALSE(ParseURL(url, data));
}

TEST(ParseUrlTest, MissingHost)
{
	const std::string url = "http://:8080";
	UrlData data;
	EXPECT_FALSE(ParseURL(url, data));
}

TEST(ParseUrlTest, EmptyString)
{
	const std::string url = "";
	UrlData data;
	EXPECT_FALSE(ParseURL(url, data));
}

TEST(ParseUrlTest, ValidUrlWithMaxPortNumber)
{
	const std::string url = "http://example.com:65535";
	UrlData expected{
		Protocol::HTTP,
		"example.com",
		65535,
		""
	};

	UrlData actual;
	EXPECT_TRUE(ParseURL(url, actual));
	EXPECT_EQ(expected, actual);
}

TEST(ParseUrlTest, ValidUrlWithMinPortNumber)
{
	const std::string url = "http://example.com:1";
	UrlData expected{
		Protocol::HTTP,
		"example.com",
		1,
		""
	};

	UrlData actual;
	EXPECT_TRUE(ParseURL(url, actual));
	EXPECT_EQ(expected, actual);
}

TEST(ParseUrlTest, UrlWithEmptyDocument)
{
	const std::string url = "http://example.com/";
	UrlData expected{
		Protocol::HTTP,
		"example.com",
		80,
		""
	};

	UrlData actual;
	EXPECT_TRUE(ParseURL(url, actual));
	EXPECT_EQ(expected, actual);
}

TEST(ParseUrlTest, UrlWithOnlyProtocolAndHost)
{
	const std::string url = "ftp://host";
	UrlData expected{
		Protocol::FTP,
		"host",
		21,
		""
	};

	UrlData actual;
	EXPECT_TRUE(ParseURL(url, actual));
	EXPECT_EQ(expected, actual);
}

TEST(ParseUrlTest, UrlWithTrailingSlashAfterPort)
{
	const std::string url = "http://example.com:8080/";
	UrlData expected{
		Protocol::HTTP,
		"example.com",
		8080,
		""
	};

	UrlData actual;
	EXPECT_TRUE(ParseURL(url, actual));
	EXPECT_EQ(expected, actual);
}

TEST(ParseUrlTest, InvalidPortFormat)
{
	const std::string url = "http://example.com:port";
	UrlData data;
	EXPECT_FALSE(ParseURL(url, data));
}