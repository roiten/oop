#include "Controller.h"

#include <algorithm>
#include <cmath>
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

	//сделать обработку команд через map

	if (action == "var")
		return DefineVar(args);
	if (action == "let")
		return SetVarValue(args);
	if (action == "fn")
		return CreateFunctionWithValue(args);
	if (action == "print")
		return PrintIdentifier(args);
	if (action == "printvars")
		return PrintAllVars(args);
	if (action == "printfns")
		return PrintAllFunctions(args);

	m_output << "Unknown command" << std::endl;
	return false;
}

bool Controller::DefineVar(const std::string& args)
{
	auto arguments = ParseArguments(args);
	if (arguments.identifierName.empty() || !arguments.firstOperand.empty() || !arguments.secondOperand.empty() || arguments.operationSymbol != ' ')
	{
		m_output << "Invalid usage\n";
		return false;
	}

	if (!m_calculator.DefineVar(arguments.identifierName))
	{
		m_output << "Name already exists" << std::endl;
		return false;
	}
	return true;
}

bool Controller::SetVarValue(const std::string& args)
{
	auto arguments = ParseArguments(args);
	if (arguments.identifierName.empty() || arguments.firstOperand.empty() || !arguments.secondOperand.empty() || arguments.operationSymbol != ' ')
	{
		m_output << "Invalid usage" << std::endl;
		return false;
	}

	if (!m_calculator.SetVarValue(arguments.identifierName, arguments.firstOperand))
	{
		m_output << "Name does not exist\n";
		return false;
	}
	return true;
}

bool Controller::CreateFunctionWithValue(const std::string& args)
{
	auto arguments = ParseArguments(args);
	if (arguments.identifierName.empty() || arguments.firstOperand.empty())
	{
		m_output << "Invalid usage" << std::endl;
		return false;
	}

	std::vector<std::string> expr;
	expr.push_back(arguments.firstOperand);
	if (!arguments.secondOperand.empty())
		expr.push_back(arguments.secondOperand);

	Calculator::Function fn = { arguments.identifierName, { arguments.operationSymbol, expr } };
	if (!m_calculator.SetFunctionValue(fn))
	{
		m_output << "Name already exists" << std::endl;
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
			m_output << "nan" << std::endl;
		else
			m_output << std::fixed << std::setprecision(2) << val << std::endl;
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
			m_output << "nan" << std::endl;
		else
			m_output << std::fixed << std::setprecision(2) << val << std::endl;
	}
	return true;
}

bool Controller::PrintIdentifier(const std::string& args)
{
	auto a = ParseArguments(args);
	if (a.identifierName.empty() || !a.firstOperand.empty() || !a.secondOperand.empty() || a.operationSymbol != ' ')
	{
		m_output << "Invalid usage" << std::endl;
		return false;
	}

	double val = m_calculator.GetIdentifierValue(a.identifierName);
	if (std::isnan(val))
		m_output << "nan" << std::endl;
	else
		m_output << std::fixed << std::setprecision(2) << val << std::endl;

	return true;
}

std::string Controller::TrimLeft(const std::string& str)
{
	size_t start = str.find_first_not_of(' ');
	return (start == std::string::npos)
		? ""
		: str.substr(start);
}

bool Controller::ReadIdentifier(std::istringstream& stream, std::string& identifier)
{
	stream >> identifier;
	return !identifier.empty();
}

std::string Controller::ExtractExpression(std::istringstream& stream, std::string& identifier)
{
	size_t eqPos = identifier.find('=');

	if (eqPos != std::string::npos)
	{
		std::string expression = identifier.substr(eqPos + 1);
		identifier = identifier.substr(0, eqPos);
		return expression;
	}

	std::string equalsSign;
	stream >> equalsSign;

	if (equalsSign != "=")
		return "";

	std::string expression;
	std::getline(stream, expression);
	return expression;
}

void Controller::RemoveSpaces(std::string& str)
{
	str.erase(std::ranges::remove(str, ' ').begin(), str.end());
}

void Controller::ParseExpression(const std::string& expression, Arguments& args)
{
	size_t operatorPos = std::string::npos;

	for (size_t i = 1; i < expression.size(); ++i)
	{
		char oper = expression[i];
		if (oper == '+' || oper == '-' || oper == '*' || oper == '/')
		{
			operatorPos = i;
			break;
		}
	}

	if (operatorPos == std::string::npos)
	{
		args.firstOperand = expression;
		return;
	}

	args.firstOperand = expression.substr(0, operatorPos);
	args.operationSymbol = expression[operatorPos];
	args.secondOperand = expression.substr(operatorPos + 1);
}

Controller::Arguments Controller::ParseArguments(const std::string& input)
{
	Arguments args{ "", ' ', "", "" };

	if (input.empty())
		return args;

	std::string trimmedInput = TrimLeft(input);
	if (trimmedInput.empty())
		return args;

	std::istringstream stream(trimmedInput);

	if (!ReadIdentifier(stream, args.identifierName))
		return args;

	std::string expression = ExtractExpression(stream, args.identifierName);
	if (expression.empty())
		return args;

	RemoveSpaces(expression);
	if (expression.empty())
		return args;

	ParseExpression(expression, args);

	return args;
}