#pragma once

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

enum class Month
{
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

enum class WeekDay
{
	Sunday = 0,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday
};

class MyDate
{

private:
	static constexpr uint64_t DAYS_EPOCH = 719468;
	static constexpr uint64_t MAX_EPOCH_DAYS = 2932896;
	static constexpr uint64_t MIN_YEAR = 1970;
	static constexpr uint64_t MAX_YEAR = 9999;

	uint64_t m_epochDays = 0;

	static constexpr bool IsLeapYear(uint64_t year)
	{
		return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	}

	static constexpr uint64_t DateToDays(uint64_t day, uint64_t month, uint64_t year)
	{
		year -= month <= 2;
		uint64_t era = year / 400;
		uint64_t yoe = year - era * 400;
		uint64_t doy = (153 * (month > 2 ? month - 3 : month + 9) + 2) / 5 + day - 1;
		uint64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
		return era * 146097 + doe - DAYS_EPOCH;
	}

	static constexpr std::tuple<uint64_t, uint64_t, uint64_t> DaysToDate(uint64_t days)
	{
		days += DAYS_EPOCH;
		uint64_t era = days / 146097;
		uint64_t doe = days - era * 146097;
		uint64_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
		uint64_t year = yoe + era * 400;
		uint64_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
		uint64_t mp = (5 * doy + 2) / 153;
		uint64_t day = doy - (153 * mp + 2) / 5 + 1;
		uint64_t month = mp < 10 ? mp + 3 : mp - 9;
		return { day, month, year + (month <= 2) };
	}

	static constexpr void ValidateDate(uint64_t day, uint64_t month, uint64_t year)
	{
		if (year < MIN_YEAR || year > MAX_YEAR)
			throw std::out_of_range("Year must be between 1970 and 9999");

		if (month < 1 || month > 12)
			throw std::out_of_range("Month must be between 1 and 12");

		uint64_t maxDay = GetDaysInMonth(month, year);
		if (day < 1 || day > maxDay)
			throw std::out_of_range("Day out of range for given month");
	}

	static constexpr uint64_t GetDaysInMonth(uint64_t month, uint64_t year)
	{
		constexpr uint64_t days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		if (month == 2 && IsLeapYear(year))
			return 29;

		return days[month - 1];
	}

public:
	constexpr MyDate() = default;

	constexpr MyDate(uint64_t day, Month month, uint64_t year)
		: MyDate(day, static_cast<uint64_t>(month), year)
	{
	}

	constexpr MyDate(uint64_t day, uint64_t month, uint64_t year)
	{
		ValidateDate(day, month, year);
		m_epochDays = DateToDays(day, month, year);
	}

	constexpr MyDate(uint64_t epochDays)
	{
		if (epochDays > MAX_EPOCH_DAYS)
			throw std::out_of_range("Epoch days out of range");
		m_epochDays = epochDays;
	}

	constexpr uint64_t GetDay() const
	{
		auto [day, month, year] = DaysToDate(m_epochDays);
		return day;
	}

	constexpr Month GetMonth() const
	{
		auto [day, month, year] = DaysToDate(m_epochDays);
		return static_cast<Month>(month);
	}

	constexpr uint64_t GetYear() const
	{
		auto [day, month, year] = DaysToDate(m_epochDays);
		return year;
	}

	constexpr WeekDay GetWeekDay() const
	{
		// 01 01 1970 = Четверг = 4
		return static_cast<WeekDay>((m_epochDays + 4) % 7);
	}

	std::string ToString() const
	{
		auto [day, month, year] = DaysToDate(m_epochDays);
		char buffer[12];
		snprintf(buffer, sizeof(buffer), "%02llu.%02llu.%04llu", day, month, year);
		return std::string(buffer);
	}

	static constexpr MyDate Min() { return MyDate(0); }
	static constexpr MyDate Max() { return MyDate(MAX_EPOCH_DAYS); }
	static MyDate Now()
	{
		using namespace std::chrono;
		auto day = duration_cast<std::chrono::days>(system_clock::now().time_since_epoch());
		return MyDate(static_cast<uint64_t>(day.count()));
	}

	constexpr MyDate operator+(int days) const
	{
		if (days < 0)
			return *this - (-days);

		uint64_t newDays = m_epochDays + static_cast<uint64_t>(days);
		if (newDays > MAX_EPOCH_DAYS)
			throw std::out_of_range("Date overflow");
		return MyDate(newDays);
	}

	constexpr MyDate operator-(int days) const
	{
		if (days < 0)
			return *this + (-days);

		if (static_cast<uint64_t>(days) > m_epochDays)
			throw std::out_of_range("Date underflow");
		return MyDate(m_epochDays - static_cast<uint64_t>(days));
	}

	constexpr MyDate& operator++()
	{
		*this = *this + 1;
		return *this;
	}

	constexpr MyDate operator++(int)
	{
		MyDate temp = *this;
		++(*this);
		return temp;
	}

	constexpr MyDate& operator--()
	{
		*this = *this - 1;
		return *this;
	}

	constexpr MyDate operator--(int)
	{
		MyDate temp = *this;
		--(*this);
		return temp;
	}


	constexpr int operator-(const MyDate& other) const
	{
		return static_cast<int>(m_epochDays) - static_cast<int>(other.m_epochDays);
	}

	constexpr MyDate& operator+=(int days)
	{
		*this = *this + days;
		return *this;
	}

	constexpr MyDate& operator-=(int days)
	{
		*this = *this - days;
		return *this;
	}

	constexpr auto operator<=>(const MyDate& other) const = default;
};

std::ostream& operator<<(std::ostream& os, const MyDate& date)
{
	return os << date.ToString();
}

std::istream& operator>>(std::istream& is, MyDate& date)
{
	uint64_t day, month, year;
	char dot1, dot2;

	is >> day >> dot1 >> month >> dot2 >> year;

	if (!is || dot1 != '.' || dot2 != '.')
	{
		is.setstate(std::ios::failbit);
		return is;
	}

	try {
		date = MyDate(day, month, year);
	}
	catch (const std::out_of_range&) {
		is.setstate(std::ios::failbit);
	}

	return is;
}
