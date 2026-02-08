#include "../src/findtext.h"
#include <catch2/catch_test_macros.hpp>
#include <fstream>

TEST_CASE("FindString returns 0 when string is found")
{
	constexpr std::string filename = "test_file.txt";

	std::ofstream testfile(filename);
	testfile << "first line\n";
	testfile << "second line with abc\n";
	testfile.close();

	const int result = FindString(filename, "abc");
	REQUIRE(result == 0);

	std::remove(filename.c_str());
}

TEST_CASE("FindString returns 1 when string is not found")
{
	constexpr std::string filename = "test_file.txt";

	std::ofstream testfile(filename);
	testfile << "first line\n";
	testfile << "second line\n";
	testfile.close();

	const int result = FindString(filename, "linee");
	REQUIRE(result == 1);

	std::remove(filename.c_str());
}

TEST_CASE("FindString returns 2 when file does not exist")
{
	const int result = FindString("nonexistent_file.txt", "abc");
	REQUIRE(result == 2);
}
