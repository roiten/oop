#pragma once
#include <map>
#include <string>
#include <vector>

class Calculator
{
public:
	using Expression = std::pair<char, std::vector<std::string>>;
	using Function = std::pair<std::string, Expression>;

	//добавить для памяти using-ги

	bool DefineVar(const std::string& varName);
	bool SetVarValue(const std::string& varName, const std::string& value);
	bool SetFunctionValue(const Function& function);
	double GetIdentifierValue(const std::string& identifierName) const;
	std::map<std::string, double> GetAllVars() const;
	std::map<std::string, double> GetAllFunctions() const;

private:
	std::vector<char> m_operators = { '+', '-', '*', '/' };

	//используем хэш таблицы
	//variableContainer, fnsContainer
	std::map<std::string, double> m_memoryVars;
	std::map<std::string, Expression> m_memoryFunctions;

	bool IsVarAlreadyExist(const std::string& varName) const;
	bool IsValidName(const std::string& varName) const;
	bool IsValidFunction(const Function& function) const;
	bool isValidOperands(const std::string& functionName, const std::vector<std::string>& operands) const;
	double CalculateFunctionValue(const std::string& functionName) const;
};
