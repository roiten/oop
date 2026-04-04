#pragma once
#include "Calculator.h"
#include <sstream>
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
	bool DefineVar(const std::string& args);
	bool SetVarValue(const std::string& args);
	bool CreateFunctionWithValue(const std::string& args);
	bool PrintIdentifier(const std::string& args);
	bool PrintAllVars(const std::string& args);
	bool PrintAllFunctions(const std::string& args);
	std::string TrimLeft(const std::string& str);
	std::string ExtractExpression(std::istringstream& stream, std::string& identifier);
	bool ReadIdentifier(std::istringstream& stream, std::string& identifier);
	void RemoveSpaces(std::string& str);
	void ParseExpression(const std::string& expression, Arguments& args);

	Calculator& m_calculator;
	std::istream& m_input;
	std::ostream& m_output;
};