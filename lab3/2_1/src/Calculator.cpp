#include "Calculator.h"

#include <cmath>
#include <regex>

bool Calculator::DefineVar(const std::string& varName)
{
	if (!IsValidName(varName) || IsVarAlreadyExist(varName))
	{
		return false;
	}

	m_memoryVars.insert(std::pair<std::string, double>(varName, NAN));
	return true;
}

double Calculator::GetIdentifierValue(const std::string& identifierName) const
{
	if (IsVarAlreadyExist(identifierName))
	{
		return m_memoryVars.contains(identifierName)
			? m_memoryVars.find(identifierName)->second
			: CalculateFunctionValue(identifierName);
	}

	return NAN;
}

std::map<std::string, double> Calculator::GetAllVars() const
{
	return m_memoryVars;
}

std::map<std::string, double> Calculator::GetAllFunctions() const
{
	std::map<std::string, double> mapFunction;
	for (const auto& element : m_memoryFunctions)
	{
		mapFunction.insert(std::pair(element.first, CalculateFunctionValue(element.first)));
	}

	return mapFunction;
}

bool Calculator::SetVarValue(const std::string& varName, const std::string& value)
{

	if (!IsVarAlreadyExist(varName))
	{
		bool varWasCreated = DefineVar(varName);
		if (!varWasCreated)
			return false;
	}

	if (std::isdigit(value[0]) || (value[0] == '-' && std::isdigit(value[1])))
	{
		m_memoryVars.find(varName)->second = std::stod(value);
		return true;
	}

	//восстановить тесты на цифры+буквы (123q)

	if (IsVarAlreadyExist(value))
	{
		m_memoryVars.find(varName)->second = m_memoryVars.find(value)->second;
	}

	return false;
}

bool Calculator::SetFunctionValue(const Function& function)
{
	if (!IsValidName(function.first))
	{
		return false;
	}

	if (IsVarAlreadyExist(function.first))
	{
		return false;
	}

	if (IsValidFunction(function))
	{
		m_memoryFunctions.insert(function);

		return true;
	}

	return false;
}

bool Calculator::IsValidFunction(const Function& function) const
{
	auto functionName = function.first;
	auto expression = function.second;

	if (expression.second.empty() || expression.second.size() > 2)
	{
		return false;
	}

	if (expression.first == ' ')
	{
		if (expression.second.size() > 1)
		{
			return false;
		}

		if (isValidOperands(functionName, expression.second))
		{
			return true;
		}

		return true;
	}

	auto it = std::find(m_operators.begin(), m_operators.end(), expression.first);

	if (it == m_operators.end() || expression.second.size() != 2)
	{
		return false;
	}

	if (isValidOperands(functionName, expression.second))
	{
		return true;
	}

	return false;
}

bool Calculator::isValidOperands(const std::string& functionName, const std::vector<std::string>& operands) const
{
	if (operands.size() == 1)
	{
		return (operands[0] != functionName) && IsVarAlreadyExist(operands[0]);
	}

	if (operands.size() == 2)
	{
		return (operands[0] != functionName || operands[1] != functionName) && IsVarAlreadyExist(operands[0]) && IsVarAlreadyExist(operands[1]);
	}

	return false;
}

bool Calculator::IsVarAlreadyExist(const std::string& varName) const
{
	if (m_memoryVars.contains(varName))
	{
		return true;
	}

	if (m_memoryFunctions.contains(varName))
	{
		return true;
	}

	return false;
}

bool Calculator::IsValidName(const std::string& varName) const
{
	if (varName.empty() || std::isdigit(varName[0]))
	{
		return false;
	}

	for (char c : varName)
	{
		if (!std::isalnum(c) && c != '_')
		{
			return false;
		}
	}
	return true;
}

double Calculator::CalculateFunctionValue(const std::string& functionName) const
{
	if (!m_memoryFunctions.contains(functionName))
	{
		return NAN;
	}

	Expression expression = m_memoryFunctions.find(functionName)->second;
	char operationSymbol = expression.first;

	if (operationSymbol == ' ')
	{
		return GetIdentifierValue(expression.second[0]);
	}

	std::vector<std::string> operandsVector = expression.second;
	double firstOperand = GetIdentifierValue(operandsVector[0]);
	double secondOperand = GetIdentifierValue(operandsVector[1]);
	double result = 0;

	if (std::isnan(firstOperand) || std::isnan(secondOperand))
	{
		return NAN;
	}

	switch (operationSymbol)
	{
	case '+':
		result = GetIdentifierValue(operandsVector[0]) + GetIdentifierValue(operandsVector[1]);
		break;
	case '-':
		result = GetIdentifierValue(operandsVector[0]) - GetIdentifierValue(operandsVector[1]);
		break;
	case '*':
		result = GetIdentifierValue(operandsVector[0]) * GetIdentifierValue(operandsVector[1]);
		break;
	case '/':
		result = GetIdentifierValue(operandsVector[1]) != 0
			? GetIdentifierValue(operandsVector[0]) / GetIdentifierValue(operandsVector[1])
			: NAN;
		break;
	}

	return result;
}