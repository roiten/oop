#pragma once
#include "Calculator.h"
#include <ostream>
#include <string>

class Controller
{
public:
	Controller(Calculator& calculator, std::istream& input, std::ostream& output);
	bool HandleCommand();

private:
	struct Arguments
	{
		std::string identifierName;
		char operationSymbol;
		std::string firstOperand;
		std::string secondOperand;
	};

	Arguments ParseArguments(const std::string& inputLine);

	bool CreateVar(const std::string& args);
	bool SetVarValue(const std::string& args);
	bool CreateFunctionWithValue(const std::string& args);
	bool PrintIdentifier(const std::string& args);
	bool PrintAllVars(const std::string& args);
	bool PrintAllFunctions(const std::string& args);

	Calculator& m_calculator;
	std::istream& m_input;
	std::ostream& m_output;
};