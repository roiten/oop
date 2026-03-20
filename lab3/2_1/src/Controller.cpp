#include "Controller.h"

#include <iomanip>
#include <sstream>

Controller::Controller(Calculator& calculator, std::istream& input, std::ostream& output)
	: m_calculator(calculator)
	, m_input(input)
	, m_output(output)
{
}

bool Controller::HandleCommand()
{
	std::string commandLine;
	if (!std::getline(m_input, commandLine))
		return false;

	std::string action, args;
	std::istringstream stream(commandLine);
	stream >> action;
	std::getline(stream, args);

	if (action == "var")      return CreateVar(args);
	if (action == "let")      return SetVarValue(args);
	if (action == "fn")       return CreateFunctionWithValue(args);
	if (action == "print")    return PrintIdentifier(args);
	if (action == "printvars") return PrintAllVars(args);
	if (action == "printfns") return PrintAllFunctions(args);

	m_output << "Unknown command\n";
	return false;
}

bool Controller::CreateVar(const std::string& args)
{
	auto a = ParseArguments(args);
	if (a.identifierName.empty() || !a.firstOperand.empty() || !a.secondOperand.empty() || a.operationSymbol != ' ')
	{
		m_output << "Invalid usage\n";
		return false;
	}

	if (!m_calculator.DefineVar(a.identifierName))
	{
		m_output << "Name already exists\n";
		return false;
	}
	return true;
}

bool Controller::SetVarValue(const std::string& args)
{
	auto a = ParseArguments(args);
	if (a.identifierName.empty() || a.firstOperand.empty() || !a.secondOperand.empty() || a.operationSymbol != ' ')
	{
		m_output << "Invalid usage\n";
		return false;
	}

	if (!m_calculator.SetVarValue(a.identifierName, a.firstOperand))
	{
		m_output << "Name does not exist\n";
		return false;
	}
	return true;
}

bool Controller::CreateFunctionWithValue(const std::string& args)
{
	auto a = ParseArguments(args);
	if (a.identifierName.empty() || a.firstOperand.empty())
	{
		m_output << "Invalid usage\n";
		return false;
	}

	std::vector<std::string> expr;
	expr.push_back(a.firstOperand);
	if (!a.secondOperand.empty())
		expr.push_back(a.secondOperand);

	Calculator::Function fn = { a.identifierName, { a.operationSymbol, expr } };
	if (!m_calculator.SetFunctionValue(fn))
	{
		m_output << "Name already exists\n";
		return false;
	}
	return true;
}

bool Controller::PrintAllVars(const std::string&)
{
	auto vars = m_calculator.GetAllVars();
	for (auto& [name, val] : vars)
	{
		m_output << name << ":";
		if (std::isnan(val))
			m_output << "nan\n";
		else
			m_output << std::fixed << std::setprecision(2) << val << "\n";
	}
	return true;
}

bool Controller::PrintAllFunctions(const std::string&)
{
	auto fns = m_calculator.GetAllFunctions();
	for (auto& [name, val] : fns)
	{
		m_output << name << ":";
		if (std::isnan(val))
			m_output << "nan\n";
		else
			m_output << std::fixed << std::setprecision(2) << val << "\n";
	}
	return true;
}

bool Controller::PrintIdentifier(const std::string& args)
{
	auto a = ParseArguments(args);
	if (a.identifierName.empty() || !a.firstOperand.empty() || !a.secondOperand.empty() || a.operationSymbol != ' ')
	{
		m_output << "Invalid usage\n";
		return false;
	}

	double val = m_calculator.GetIdentifierValue(a.identifierName);
	if (std::isnan(val))
		m_output << "nan\n";
	else
		m_output << std::fixed << std::setprecision(2) << val << "\n";

	return true;
}

Controller::Arguments Controller::ParseArguments(const std::string& inputLine)
{
	Arguments a;
	a.operationSymbol = ' ';

	if (inputLine.empty()) return a;

	std::string line = inputLine;
	size_t start = line.find_first_not_of(' ');
	if (start == std::string::npos) return a;
	line = line.substr(start);

	std::istringstream stream(line);
	stream >> a.identifierName;
	if (a.identifierName.empty()) return a;

	size_t eqPos = a.identifierName.find('=');
	std::string rest;
	if (eqPos != std::string::npos)
	{
		rest = a.identifierName.substr(eqPos + 1);
		a.identifierName = a.identifierName.substr(0, eqPos);
	}
	else
	{
		std::string eq;
		stream >> eq;
		if (eq != "=") return a;
		std::getline(stream, rest);
	}

	rest.erase(std::remove(rest.begin(), rest.end(), ' '), rest.end());
	if (rest.empty()) return a;

	size_t opPos = std::string::npos;
	for (size_t i = 1; i < rest.size(); ++i)
	{
		char c = rest[i];
		if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			opPos = i;
			break;
		}
	}

	if (opPos == std::string::npos)
	{
		a.firstOperand = rest;
	}
	else
	{
		a.firstOperand = rest.substr(0, opPos);
		a.operationSymbol = rest[opPos];
		a.secondOperand = rest.substr(opPos + 1);
	}

	return a;
}