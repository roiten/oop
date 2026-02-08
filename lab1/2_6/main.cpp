#include <iostream>

int CharToDigit(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	return -1;
}

char DigitToChar(int digit)
{
	if (digit >= 0 && digit <= 9)
		return static_cast<char>('0' + digit);
	if (digit >= 10 && digit <= 35)
		return static_cast<char>('A' + digit - 10);
	return '?';
}

int StringToInt(std::string str, const int radix, bool& wasError)
{
	if (radix < 2 || radix > 36 || str.empty())
	{
		wasError = true;
		return 0;
	}

	int sign = 1;
	if (str[0] == '-')
	{
		sign = -1;
		str.erase(str.begin());

		if (str.empty())
		{
			wasError = true;
			return 0;
		}
	}

	int result = 0;
	int power = 1;

	while (!str.empty())
	{
		const int digit = CharToDigit(str.back());

		if (digit >= radix)
		{
			wasError = true;
			return 0;
		}

		if (digit != 0 && power > (INT_MAX - result) / digit)
		{
			wasError = true;
			return 0;
		}
		result += digit * power;

		if (!str.empty() && power > INT_MAX / radix)
		{
			wasError = true;
			return 0;
		}

		power *= radix;
		str.pop_back();
	}

	return result * sign;
}

std::string IntToString(int decNumber, int to, bool &wasError)
{
	std::vector<int> result;
	std::string answer;

	std::string sign = "";
	if (decNumber < 0)
	{
		sign = "-";
		decNumber *= -1;
	}

	while (decNumber > 0)
	{
		result.push_back(decNumber % to);
		//обработка ошибок
		decNumber /= to;
	}

	if (result.empty())
	{
		result.push_back(0);
	}

	answer += sign;
	for (int i = result.size() - 1; i >= 0; --i)
	{
		auto digit = DigitToChar(result[i]);
		if (digit != '?') answer += digit;
		else
		{
			wasError = true;
			return "";
		}
	}


	return answer;
}

int main(int argc, char* argv[])
{
	if (argc != 4) {
		std::cerr << "Usage: radix.exe <from> <to> <value>\n";
		return 1;
	}

	bool wasError = false;

	int from = StringToInt(argv[1], 10, wasError);
	if (wasError) {
		std::cerr << "Invalid source radix\n";
		return 1;
	}

	int to = StringToInt(argv[2], 10, wasError);
	if (wasError) {
		std::cerr << "Invalid destination radix\n";
		return 1;
	}

	int value = StringToInt(argv[3], from, wasError);
	if (wasError) {
		std::cerr << "Invalid input value\n";
		return 1;
	}

	std::string answer = IntToString(value, to, wasError);
	if (wasError) {
		std::cerr << "Conversion error\n";
		return 1;
	}

	std::cout << answer << '\n';
	return 0;
}