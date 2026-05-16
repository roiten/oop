#include "../MyDate.h"
#include "catch2/catch_test_macros.hpp"
#include <chrono>
#include <sstream>
#include <stdexcept>

void AssertDateEquals(const MyDate& date, uint64_t expectedDay, Month expectedMonth,
	uint64_t expectedYear, WeekDay expectedWeekDay)
{
	REQUIRE(date.GetDay() == expectedDay);
	REQUIRE(date.GetMonth() == expectedMonth);
	REQUIRE(date.GetYear() == expectedYear);
	REQUIRE(date.GetWeekDay() == expectedWeekDay);
}

void AssertDateEquals(const MyDate& date, const MyDate& expected)
{
	AssertDateEquals(date, expected.GetDay(), expected.GetMonth(),
		expected.GetYear(), expected.GetWeekDay());
}

TEST_CASE("Constructor Tests")
{

	SECTION("Empty constructor")
	{
		constexpr MyDate date;
		AssertDateEquals(date, 1, Month::January, 1970, WeekDay::Thursday);
	}

	SECTION("Constructor from day-month-year")
	{
		constexpr MyDate date(11, Month::September, 2001);
		AssertDateEquals(date, 11, Month::September, 2001, WeekDay::Tuesday);
	}

	SECTION("Constructor from day-month-year")
	{
		constexpr MyDate date(15, 5, 2020);
		AssertDateEquals(date, 15, Month::May, 2020, WeekDay::Friday);
	}

	SECTION("Constructor from epoch days - min (0)")
	{
		constexpr MyDate date(0);
		AssertDateEquals(date, 1, Month::January, 1970, WeekDay::Thursday);
	}

	SECTION("Constructor from epoch days - middle (365)")
	{
		constexpr MyDate date(365);
		AssertDateEquals(date, 1, Month::January, 1971, WeekDay::Friday);
	}

	SECTION("Constructor from epoch days - max (2932896)")
	{
		constexpr MyDate date(2932896);
		AssertDateEquals(date, 31, Month::December, 9999, WeekDay::Friday);
	}

	SECTION("Constructor - invalid timestamp throws")
	{
		REQUIRE_THROWS_AS(MyDate(2932897), std::out_of_range);
		REQUIRE_THROWS_AS(MyDate(2932896 + 365), std::out_of_range);
	}

	SECTION("Valid date - leap year (29.02.2020)")
	{
		constexpr MyDate date(29, Month::February, 2020);
		AssertDateEquals(date, 29, Month::February, 2020, WeekDay::Saturday);
	}

	SECTION("Invalid date - non-leap year throws (29.02.2019)")
	{
		REQUIRE_THROWS_AS(MyDate(29, Month::February, 2019), std::out_of_range);
	}

	SECTION("Leap year century - 2000 is leap (29.02.2000)")
	{
		constexpr MyDate date(29, Month::February, 2000);
		AssertDateEquals(date, 29, Month::February, 2000, WeekDay::Tuesday);
	}

	SECTION("Leap year century - 1900 not leap throws")
	{
		REQUIRE_THROWS_AS(MyDate(29, Month::February, 1900), std::out_of_range);
	}

	SECTION("Invalid day throws")
	{
		REQUIRE_THROWS_AS(MyDate(0, Month::July, 2005), std::out_of_range);
		REQUIRE_THROWS_AS(MyDate(32, Month::July, 2005), std::out_of_range);
		REQUIRE_THROWS_AS(MyDate(32, 12, 2005), std::out_of_range);
	}

	SECTION("Invalid month throws")
	{
		REQUIRE_THROWS_AS(MyDate(1, 0, 2020), std::out_of_range);
		REQUIRE_THROWS_AS(MyDate(1, 13, 2020), std::out_of_range);
	}

	SECTION("Invalid year throws")
	{
		REQUIRE_THROWS_AS(MyDate(1, Month::January, 1969), std::out_of_range);
		REQUIRE_THROWS_AS(MyDate(1, Month::January, 10000), std::out_of_range);
		REQUIRE_THROWS_AS(MyDate(1, 1, 1969), std::out_of_range);
		REQUIRE_THROWS_AS(MyDate(1, 1, 10000), std::out_of_range);
	}

	SECTION("Valid year boundaries")
	{
		REQUIRE_NOTHROW(MyDate(1, Month::January, 1970));
		REQUIRE_NOTHROW(MyDate(31, Month::December, 9999));
		REQUIRE_NOTHROW(MyDate(1, 1, 1970));
		REQUIRE_NOTHROW(MyDate(31, 12, 9999));
	}
}

TEST_CASE("WeekDay Tests")
{

	SECTION("start (01.01.1970 = Thursday)")
	{
		MyDate date(1, Month::January, 1970);
		REQUIRE(date.GetWeekDay() == WeekDay::Thursday);
	}

	SECTION("08.01.1970 = Thursday")
	{
		MyDate date(8, Month::January, 1970);
		REQUIRE(date.GetWeekDay() == WeekDay::Thursday);
	}

	SECTION("02.01.1970 = Friday")
	{
		MyDate date(2, Month::January, 1970);
		REQUIRE(date.GetWeekDay() == WeekDay::Friday);
	}

	SECTION("31.12.9999 = Friday")
	{
		MyDate date(31, Month::December, 9999);
		REQUIRE(date.GetWeekDay() == WeekDay::Friday);
	}
}

TEST_CASE("Increment-Decrement")
{

	SECTION("Prefix increment")
	{
		MyDate date(1, Month::July, 2005);
		const auto nextDay = ++date;
		AssertDateEquals(nextDay, 2, Month::July, 2005, WeekDay::Saturday);
		AssertDateEquals(date, 2, Month::July, 2005, WeekDay::Saturday);
	}

	SECTION("Postfix increment (old value)")
	{
		MyDate date(1, Month::July, 2005);
		const auto currDay = date++;
		AssertDateEquals(currDay, 1, Month::July, 2005, WeekDay::Friday);
		AssertDateEquals(date, 2, Month::July, 2005, WeekDay::Saturday);
	}

	SECTION("Prefix decrement")
	{
		MyDate date(2, Month::July, 2005);
		const auto prevDay = --date;
		AssertDateEquals(prevDay, 1, Month::July, 2005, WeekDay::Friday);
		AssertDateEquals(date, 1, Month::July, 2005, WeekDay::Friday);
	}

	SECTION("Postfix decrement (old value)")
	{
		MyDate date(2, Month::July, 2005);
		const auto currDay = date--;
		AssertDateEquals(currDay, 2, Month::July, 2005, WeekDay::Saturday);
		AssertDateEquals(date, 1, Month::July, 2005, WeekDay::Friday);
	}

	SECTION("Increment through month boundary (31.01.2005 -> 01.02.2005)")
	{
		MyDate date(31, Month::January, 2005);
		++date;
		AssertDateEquals(date, 1, Month::February, 2005, WeekDay::Tuesday);
	}

	SECTION("Increment through year boundary (31.12.2005 -> 01.01.2006)")
	{
		MyDate date(31, Month::December, 2005);
		++date;
		AssertDateEquals(date, 1, Month::January, 2006, WeekDay::Sunday);
	}

	SECTION("Decrement through month boundary (01.03.2005 -> 28.02.2005)")
	{
		MyDate date(1, Month::March, 2005);
		--date;
		AssertDateEquals(date, 28, Month::February, 2005, WeekDay::Monday);
	}

	SECTION("Decrement through year boundary (01.01.2005 -> 31.12.2004)")
	{
		MyDate date(1, Month::January, 2005);
		--date;
		AssertDateEquals(date, 31, Month::December, 2004, WeekDay::Friday);
	}

	SECTION("Increment through February in leap year")
	{
		MyDate date(28, Month::February, 2020);
		++date;
		AssertDateEquals(date, 29, Month::February, 2020, WeekDay::Saturday);
		++date;
		AssertDateEquals(date, 1, Month::March, 2020, WeekDay::Sunday);
	}

	SECTION("Increment through February in non-leap year")
	{
		MyDate date(28, Month::February, 2019);
		++date;
		AssertDateEquals(date, 1, Month::March, 2019, WeekDay::Friday);
	}

	SECTION("Prefix increment overflow Max throws")
	{
		MyDate date = MyDate::Max();
		REQUIRE_THROWS_AS(++date, std::out_of_range);
		AssertDateEquals(date, MyDate::Max());
	}

	SECTION("Postfix increment overflow Max throws")
	{
		MyDate date = MyDate::Max();
		REQUIRE_THROWS_AS(date++, std::out_of_range);
		AssertDateEquals(date, MyDate::Max());
	}

	SECTION("Prefix decrement underflow on Min throws")
	{
		MyDate date = MyDate::Min();
		REQUIRE_THROWS_AS(--date, std::out_of_range);
		AssertDateEquals(date, MyDate::Min());
	}

	SECTION("Postfix decrement underflow Min throws")
	{
		MyDate date = MyDate::Min();
		REQUIRE_THROWS_AS(date--, std::out_of_range);
		AssertDateEquals(date, MyDate::Min());
	}
}

TEST_CASE("Addition Tests")
{

	SECTION("Add positive days")
	{
		MyDate date(1, Month::July, 2005);
		auto newDate = date + 30;
		AssertDateEquals(newDate, 31, Month::July, 2005, WeekDay::Sunday);
	}

	SECTION("Add negative days")
	{
		MyDate date(15, Month::June, 2020);
		auto newDate = date + (-5);
		AssertDateEquals(newDate, 10, Month::June, 2020, WeekDay::Wednesday);
	}

	SECTION("Add zero days")
	{
		MyDate date(15, Month::August, 2023);
		auto newDate = date + 0;
		AssertDateEquals(newDate, date);
	}

	SECTION("Add days (01.07.2005 + 31 = 01.08.2005)")
	{
		MyDate date(1, Month::July, 2005);
		auto newDate = date + 31;
		AssertDateEquals(newDate, 1, Month::August, 2005, WeekDay::Monday);
	}

	SECTION("Add days - year transition")
	{
		MyDate date(1, Month::December, 2005);
		auto newDate = date + 31;
		AssertDateEquals(newDate, 1, Month::January, 2006, WeekDay::Sunday);
	}

	SECTION("Add large number of days")
	{
		MyDate date(1, Month::January, 1970);
		auto newDate = date + 3677;
		REQUIRE(newDate.GetYear() == 1980);
	}

	SECTION("Add overflow (Max() + 1 throws)")
	{
		MyDate date = MyDate::Max();
		REQUIRE_THROWS_AS(date + 1, std::out_of_range);
		AssertDateEquals(date, MyDate::Max());
	}

	SECTION("Add overflow with large number")
	{
		MyDate date(31, Month::December, 9998);
		REQUIRE_THROWS_AS(date + 366, std::out_of_range);
		AssertDateEquals(date, 31, Month::December, 9998, WeekDay::Thursday);
	}
}


TEST_CASE("Subtraction Tests")
{
	SECTION("Subtract positive days")
	{
		MyDate date(3, Month::July, 2005);
		auto newDate = date - 2;
		AssertDateEquals(newDate, 1, Month::July, 2005, WeekDay::Friday);
	}

	SECTION("Subtract negative days (add)")
	{
		MyDate date(10, Month::June, 2020);
		auto newDate = date - (-5);
		AssertDateEquals(newDate, 15, Month::June, 2020, WeekDay::Monday);
	}

	SECTION("Subtract zero days (no change)")
	{
		MyDate date(15, Month::August, 2023);
		auto newDate = date - 0;
		AssertDateEquals(newDate, date);
	}

	SECTION("Subtract days - month transition")
	{
		MyDate date(1, Month::July, 2005);
		auto newDate = date - 1;
		AssertDateEquals(newDate, 30, Month::June, 2005, WeekDay::Thursday);
	}

	SECTION("Subtract days - year transition")
	{
		MyDate date(1, Month::January, 2005);
		auto newDate = date - 1;
		AssertDateEquals(newDate, 31, Month::December, 2004, WeekDay::Friday);
	}

	SECTION("Subtract large number of days")
	{
		MyDate date(1, Month::January, 1980);
		auto newDate = date - 3650;
		REQUIRE(newDate.GetYear() == 1970);
	}

	SECTION("Subtract underflow (Min() - 1 throws)")
	{
		MyDate date = MyDate::Min();
		REQUIRE_THROWS_AS(date - 1, std::out_of_range);
		AssertDateEquals(date, MyDate::Min());
	}
}

TEST_CASE("Date Difference Tests")
{

	SECTION(" Positive difference (05.07.2025 - 01.07.2025 = 4)")
	{
		MyDate date1(1, Month::July, 2025);
		MyDate date2(5, Month::July, 2025);
		REQUIRE((date2 - date1) == 4);
	}

	SECTION("Negative difference (01.01.1980 - 01.01.1990 < 0)")
	{
		MyDate date1(1, Month::January, 1980);
		MyDate date2(1, Month::January, 1990);
		REQUIRE((date1 - date2) < 0);
		REQUIRE((date2 - date1) > 0);
	}

	SECTION("Zero difference")
	{
		MyDate date1(22, Month::January, 2011);
		MyDate date2(22, Month::January, 2011);
		REQUIRE((date1 - date2) == 0);
	}
}


TEST_CASE("Compound Assignment Tests")
{

	SECTION("Add assign (date += 30)")
	{
		MyDate date(1, Month::July, 2005);
		date += 30;
		AssertDateEquals(date, 31, Month::July, 2005, WeekDay::Sunday);
	}

	SECTION("Subtract assign (date -= 2)")
	{
		MyDate date(3, Month::July, 2005);
		date -= 2;
		AssertDateEquals(date, 1, Month::July, 2005, WeekDay::Friday);
	}

	SECTION("Add assign with no change")
	{
		MyDate date(15, Month::August, 2023);
		MyDate copy = date;
		date += 0;
		AssertDateEquals(date, copy);
	}

	SECTION("Subtract assign with no change")
	{
		MyDate date(15, Month::August, 2023);
		MyDate copy = date;
		date -= 0;
		AssertDateEquals(date, copy);
	}

	SECTION("Chain operations")
	{
		MyDate date(1, Month::January, 2005);
		date += 10;
		date += 5;
		date -= 6;
		AssertDateEquals(date, 10, Month::January, 2005, WeekDay::Monday);
	}

	SECTION("Add assign overflow (Max() += 1 throw)")
	{
		MyDate date = MyDate::Max();
		REQUIRE_THROWS_AS(date += 1, std::out_of_range);
		AssertDateEquals(date, MyDate::Max());
	}

	SECTION("Subtract assign underflow (Min -= 1 throws)")
	{
		MyDate date = MyDate::Min();
		REQUIRE_THROWS_AS(date -= 1, std::out_of_range);
		AssertDateEquals(date, MyDate::Min());
	}
}

TEST_CASE("Comparison")
{
	MyDate d1(22, Month::January, 2006);
	MyDate d2(22, Month::January, 2006);
	MyDate d3(22, Month::February, 2006);
	MyDate d4(23, Month::January, 2006);
	MyDate d5(22, Month::January, 2005);

	SECTION("Equality (==)")
	{
		REQUIRE(d1 == d2);
		REQUIRE_FALSE(d1 == d3);
	}

	SECTION("Inequality (!=)")
	{
		REQUIRE(d1 != d3);
		REQUIRE_FALSE(d1 != d2);
	}

	SECTION("Less (<)")
	{
		REQUIRE(d1 < d3);
		REQUIRE(d5 < d1);
		REQUIRE_FALSE(d3 < d1);
	}

	SECTION("Greater (>)")
	{
		REQUIRE(d3 > d1);
		REQUIRE(d1 > d5);
		REQUIRE_FALSE(d1 > d3);
	}

	SECTION("Less than or equal (<=)")
	{
		REQUIRE(d1 <= d2);
		REQUIRE(d1 <= d3);
		REQUIRE(d1 <= d1);
	}

	SECTION("Greater than or equal (>=)")
	{
		REQUIRE(d2 >= d1);
		REQUIRE(d3 >= d1);
		REQUIRE(d1 >= d1);
	}

	SECTION("Spaceship operator (<=>)")
	{
		REQUIRE((d1 <=> d2) == 0);
		REQUIRE((d1 <=> d3) < 0);
		REQUIRE((d3 <=> d1) > 0);
	}
}

TEST_CASE("Static Methods Tests")
{

	SECTION("Min() returns 01.01.1970")
	{
		constexpr auto minDate = MyDate::Min();
		AssertDateEquals(minDate, 1, Month::January, 1970, WeekDay::Thursday);
	}

	SECTION("Max() returns 31.12.9999")
	{
		constexpr auto maxDate = MyDate::Max();
		AssertDateEquals(maxDate, 31, Month::December, 9999, WeekDay::Friday);
	}
}


TEST_CASE("ToString Tests")
{
	SECTION("ToString with single digits")
	{
		MyDate date(9, Month::May, 2024);
		REQUIRE(date.ToString() == "09.05.2024");
	}

	SECTION("ToString with double digits")
	{
		MyDate date(5, Month::December, 2023);
		REQUIRE(date.ToString() == "05.12.2023");
	}
}


TEST_CASE("Output Operator Tests")
{

	SECTION("Output valid date (05.06.2024)")
	{
		MyDate date(5, Month::June, 2024);
		std::stringstream ss;
		ss << date;
		REQUIRE(ss.str() == "05.06.2024");
	}

	SECTION("Output min date (01.01.1970)")
	{
		std::stringstream ss;
		ss << MyDate::Min();
		REQUIRE(ss.str() == "01.01.1970");
	}

	SECTION("Output max date (31.12.9999)")
	{
		std::stringstream ss;
		ss << MyDate::Max();
		REQUIRE(ss.str() == "31.12.9999");
	}
}

TEST_CASE("Input Operator Tests")
{

	SECTION("Valid format (12.03.2020)")
	{
		std::stringstream ss("12.03.2020");
		MyDate date;
		ss >> date;
		REQUIRE_FALSE(ss.fail());
		AssertDateEquals(date, 12, Month::March, 2020, WeekDay::Thursday);
	}

	SECTION("Input min date (01.01.1970)")
	{
		std::stringstream ss("01.01.1970");
		MyDate date;
		ss >> date;
		REQUIRE_FALSE(ss.fail());
		AssertDateEquals(date, MyDate::Min());
	}

	SECTION("Input max date (31.12.9999)")
	{
		std::stringstream ss("31.12.9999");
		MyDate date;
		ss >> date;
		REQUIRE_FALSE(ss.fail());
		AssertDateEquals(date, MyDate::Max());
	}

	SECTION("Input with single digits (5.3.2020)")
	{
		std::stringstream ss("5.3.2020");
		MyDate date;
		ss >> date;
		REQUIRE_FALSE(ss.fail());
		AssertDateEquals(date, 5, Month::March, 2020, WeekDay::Thursday);
	}

	SECTION("With whitespaces")
	{
		std::stringstream ss("  15.08.2025  ");
		MyDate date;
		ss >> date;
		REQUIRE_FALSE(ss.fail());
		AssertDateEquals(date, 15, Month::August, 2025, WeekDay::Friday);
	}

	SECTION("Invalid separator")
	{
		std::stringstream ss("31-12-2020");
		MyDate date;
		ss >> date;
		REQUIRE(ss.fail());
	}

	SECTION("Invalid separator (slash) set failbit")
	{
		std::stringstream ss("31/12/2020");
		MyDate date;
		ss >> date;
		REQUIRE(ss.fail());
	}

	SECTION("Missing parts (01.07 set failbit)")
	{
		std::stringstream ss("01.07");
		MyDate date;
		ss >> date;
		REQUIRE(ss.fail());
	}

	SECTION("Invalid date values (32.01.2020 set failbit)")
	{
		std::stringstream ss("32.01.2020");
		MyDate date;
		ss >> date;
		REQUIRE(ss.fail());
	}
}