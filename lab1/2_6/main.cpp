#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <stdexcept>
#include <cctype>

int CharToDigit(char c)
{
	c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));

    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    throw std::invalid_argument("Invalid character in number");
}

char DigitToChar(int digit)
{
    if (digit >= 0 && digit <= 9)
        return static_cast<char>('0' + digit);
    if (digit >= 10 && digit <= 35)
        return static_cast<char>('A' + digit - 10);
    throw std::invalid_argument("Invalid digit for conversion");
}

int StringToInt(const std::string& convertible, const int radix)
{
    if (radix < 2 || radix > 36)
        throw std::invalid_argument("Radix must be between 2 and 36");

    if (convertible.empty())
        throw std::invalid_argument("Empty string");

    int sign = 1;
    size_t startPos = 0;

    if (convertible[0] == '-')
    {
        sign = -1;
        startPos = 1;

        if (convertible.length() == 1)
            throw std::invalid_argument("Missing digits after minus sign");
    }

    int result = 0;
    int power = 1;

	for (size_t i = convertible.length(); i > startPos; )
	{
		--i;

		const int digit = CharToDigit(convertible[i]);

		if (digit >= radix)
			throw std::invalid_argument("Digit exceeds radix");

		if (digit != 0 && power > (INT_MAX - result) / digit)
			throw std::overflow_error("Integer overflow");

		result += digit * power;

		if (i > startPos && power > INT_MAX / radix)
			throw std::overflow_error("Integer overflow");

		if (i > startPos)
			power *= radix;
	}

    return result * sign;
}

std::string IntToString(int decNumber, int radix)
{
    if (radix < 2 || radix > 36)
        throw std::invalid_argument("Radix must be between 2 and 36");

    if (decNumber == 0)
        return "0";

    std::vector<int> digits;
    std::string result;
    unsigned int number = (decNumber < 0) ? -decNumber : decNumber;

    while (number > 0)
    {
        digits.push_back(number % radix);
        number /= radix;
    }

    if (decNumber < 0)
        result += '-';

    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i)
    {
        result += DigitToChar(digits[i]);
    }

    return result;
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 4) {
            std::cerr << "Usage: radix <from> <to> <value>\n";
            return 1;
        }

        int from = StringToInt(argv[1], 10);
        int to = StringToInt(argv[2], 10);
        int value = StringToInt(argv[3], from);

        std::string answer = IntToString(value, to);
    	std::cout << answer << '\n';

        return 0;
    }
    catch (const std::invalid_argument& exception)
    {
        std::cerr << "Invalid argument: " << exception.what() << '\n';
        return 1;
    }
    catch (const std::overflow_error& exception)
    {
        std::cerr << "Overflow error: " << exception.what() << '\n';
        return 1;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Error: " << exception.what() << '\n';
        return 1;
    }
}