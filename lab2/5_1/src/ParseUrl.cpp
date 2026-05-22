#include "ParseUrl.h"
#include <algorithm>
#include <regex>
#include <stdexcept>
#include <unordered_map>

constexpr int MIN_PORT = 1;
constexpr int MAX_PORT = 65535;

const std::unordered_map<std::string, Protocol> PROTOCOL_MAP = {
	{ "http", Protocol::HTTP },
	{ "https", Protocol::HTTPS },
	{ "ftp", Protocol::FTP }
};

const std::unordered_map<Protocol, int> PROTOCOL_DEFAULT_PORT = {
	{ Protocol::HTTP, 80 },
	{ Protocol::HTTPS, 443 },
	{ Protocol::FTP, 21 }
};

void CheckIsPortInRange(const int port)
{
	if (port < MIN_PORT || port > MAX_PORT)
	{
		throw std::runtime_error("Порт должен быть в диапазоне от "
			+ std::to_string(MIN_PORT) + " и до " + std::to_string(MAX_PORT));
	}
}

std::string ToLower(const std::string& str)
{
	std::string result(str.size(), '\0');

	std::ranges::transform(str, result.begin(),
		[](const char ch) { return static_cast<char>(std::tolower(ch)); });

	return result;
}

Protocol ParseProtocol(const std::string& protocolStr)
{
	const auto it = PROTOCOL_MAP.find(ToLower(protocolStr));
	if (it == PROTOCOL_MAP.end())
	{
		throw std::runtime_error("Неизвестный протокол: " + protocolStr);
	}
	return it->second;
}

int ParsePort(const std::string& portStr, const Protocol protocol)
{
	if (portStr.empty())
	{
		const auto it = PROTOCOL_DEFAULT_PORT.find(protocol);
		if (it == PROTOCOL_DEFAULT_PORT.end())
		{
			throw std::runtime_error("Не удалось найти стандартный порт");
		}
		return it->second;
	}

	const int port = std::stoi(portStr);
	CheckIsPortInRange(port);

	return port;
}

bool ParseURL(const std::string& url, UrlData& data)
{
	UrlData temp = data;
	auto& [protocol, host, port, document] = temp;

	try
	{
		std::regex urlRegex(R"(^([a-zA-Z]+):\/\/([a-zA-Z0-9\.\-]+)(?::([0-9]{1,5}))?(?:\/([^?#\s]*))?(?:\?([^#\s]*))?(?:#(\S*))?$)", std::regex::icase);

		std::smatch match;
		if (!std::regex_match(url, match, urlRegex))
		{
			throw std::runtime_error("Регулярное выражение не выполнилось");
		}

		protocol = ParseProtocol(match[1]);
		host = match[2];
		port = ParsePort(match[3], protocol);
		document = match[4].matched ? match[4].str() : "";
		data = temp;
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}