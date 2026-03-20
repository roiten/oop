#include "catch2/catch_test_macros.hpp"
#include "../src/Calculator.h"

TEST_CASE("Incorrect name of identifier")
{
	SECTION("A calculator")
	{
		Calculator calc;

		WHEN("create new var with value 1 and with name superVar123")
		{
			calc.SetVarValue("superVar123", "1");

			THEN("new var`s value is NaN")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("superVar")));
			}
		}

		WHEN("create new var with value -10 and with name _anotherVar")
		{
			calc.SetVarValue("_anotherVar", "-10");

			THEN("new var`s value is -10")
			{
				REQUIRE(calc.GetIdentifierValue("_anotherVar") == -10);
			}
		}

		WHEN("names of vars are the same")
		{
			calc.DefineVar("x");
			calc.DefineVar("x");

			THEN("exist only first x")
			{
				REQUIRE(calc.GetAllVars().size() == 1);
			}
		}

		WHEN("create new function with value x and with name _123x")
		{
			calc.SetVarValue("x", "1");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(' ', { "x" })));

			THEN("fn exist")
			{
				REQUIRE(calc.GetAllFunctions().size() == 1);
			}
		}

		WHEN("function and var names are the same")
		{
			calc.SetVarValue("x", "1");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("x", std::pair<char, std::vector<std::string>>(' ', { "x" })));

			THEN("only one identifier exist")
			{
				REQUIRE(calc.GetAllFunctions().empty());
				REQUIRE(calc.GetAllVars().size() == 1);
			}
		}
	}
}

TEST_CASE("Creating vars with values of different vars")
{
	SECTION("A calculator")
	{
		Calculator calc;

		WHEN("create new var with value of var y")
		{
			calc.DefineVar("x");
			calc.SetVarValue("y", "1");

			calc.SetVarValue("x", "y");

			THEN("values of vars are the equal")
			{
				REQUIRE(calc.GetIdentifierValue("x") == 1);
				REQUIRE(calc.GetIdentifierValue("y") == 1);
				REQUIRE(calc.GetAllVars().size() == 2);
			}
		}

		WHEN("create var y with value of var x(NaN)")
		{
			calc.DefineVar("x");
			calc.SetVarValue("y", "x");

			THEN("values of both vars is NaN")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("x")));
				REQUIRE(std::isnan(calc.GetIdentifierValue("y")));
				REQUIRE(calc.GetAllVars().size() == 2);
			}
		}
	}
}

TEST_CASE("Creating functions")
{
	SECTION("A calculator")
	{
		Calculator calc;

		WHEN("creating function without values in field params(expected 1 param)")
		{
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(' ', {  })));

			THEN("fn does not exist")
			{
				REQUIRE(calc.GetAllFunctions().size() == 0);
			}
		}

		WHEN("creating function with 3 different values in field params(expected 1 param)")
		{
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(' ', { "x", "y", "z" })));

			THEN("fn does not exist")
			{
				REQUIRE(calc.GetAllFunctions().size() == 0);
			}
		}

		WHEN("creating function with 2 different values in field params and without operator(expected 1 param)")
		{
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(' ', { "x", "y" })));

			THEN("fn does not exist")
			{
				REQUIRE(calc.GetAllFunctions().size() == 0);
			}
		}

		WHEN("creating function with 1 value in field params and with '+' operator(expected 2 param)")
		{
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "x" })));

			THEN("fn does not exist")
			{
				REQUIRE(calc.GetAllFunctions().size() == 0);
			}
		}

		WHEN("creating function with 2 value in field params and with '+' operator")
		{
			calc.DefineVar("x");
			calc.DefineVar("x");

			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "x", "x" })));

			THEN("fn exists")
			{
				REQUIRE(calc.GetAllFunctions().size() == 1);
			}
		}

		WHEN("creating function with 2 value in field params and with ';' operator")
		{
			calc.DefineVar("x");
			calc.DefineVar("x");

			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(';', { "x", "x" })));

			THEN("fn does not exist")
			{
				REQUIRE(calc.GetAllFunctions().size() == 0);
			}
		}

		WHEN("creating function with 2 value, that undefined in field params and with '+' operator")
		{
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(';', { "x", "y" })));

			THEN("fn does not exist")
			{
				REQUIRE(calc.GetAllFunctions().size() == 0);
			}
		}

		WHEN("create new function fn = x and fn1 = fn + x, where x = NaN")
		{
			calc.DefineVar("x");

			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(' ', { "x" })));
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn1", std::pair<char, std::vector<std::string>>('+', { "x", "fn" })));

			THEN("add fn = x and fn1 = fn + x")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("x")));
				REQUIRE(calc.GetAllFunctions().size() == 2);
			}
		}

		WHEN("x = NaN; fn = x; fn1 = fn + 1")
		{
			calc.DefineVar("x");

			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(' ', { "x" })));
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn1", std::pair<char, std::vector<std::string>>('+', { "1", "fn" })));

			THEN("add fn = x and fn1 = fn + x")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("x")));
				REQUIRE(calc.GetAllFunctions().size() == 1);
			}
		}

		WHEN("x = NaN; y = NaN; function without identifier = x + y")
		{
			calc.DefineVar("x");
			calc.DefineVar("y");

			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("", std::pair<char, std::vector<std::string>>('+', { "x", "y" })));

			THEN("functions memory is empty")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("x")));
				REQUIRE(std::isnan(calc.GetIdentifierValue("y")));
				REQUIRE(calc.GetAllFunctions().size() == 0);
			}
		}
	}
}

TEST_CASE("Value functions output")
{
	GIVEN("A calculator")
	{
		Calculator calc;

		WHEN("x = 1; y = 2; fn = x + y")
		{
			calc.SetVarValue("x", "1");
			calc.SetVarValue("y", "2");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "x", "y" })));

			THEN("fn = 3")
			{
				REQUIRE(calc.GetIdentifierValue("fn") == 3);
			}
		}

		WHEN("x = 1; y = 2; fn = fn + y")
		{
			calc.SetVarValue("x", "1");
			calc.SetVarValue("y", "2");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "fn", "y" })));

			THEN("fn = NaN")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("fn")));
			}
		}

		WHEN("x = 1; y = 2; fn = x + fn")
		{
			calc.SetVarValue("x", "1");
			calc.SetVarValue("y", "2");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "x", "fn" })));

			THEN("fn = NaN")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("fn")));
			}
		}

		WHEN("x = 1; fn = fn")
		{
			calc.SetVarValue("x", "1");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "fn" })));

			THEN("fn = NaN")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("fn")));
			}
		}

		WHEN("x = 1; y = 2;")
		{
			calc.SetVarValue("x", "1");
			calc.SetVarValue("y", "2");

			THEN("fn = NaN")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("fn")));
			}
		}

		WHEN("x = 2; y = 2; fn = x - y")
		{
			calc.SetVarValue("x", "2");
			calc.SetVarValue("y", "2");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('-', { "x", "y" })));

			THEN("fn = 0")
			{
				REQUIRE(calc.GetIdentifierValue("fn") == 0);
			}
		}

		WHEN("x = 2; y = 2; fn = x / y")
		{
			calc.SetVarValue("x", "2");
			calc.SetVarValue("y", "2");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('/', { "x", "y" })));

			THEN("fn = 1")
			{
				REQUIRE(calc.GetIdentifierValue("fn") == 1);
			}
		}

		WHEN("x = 2; y = 0; fn = x / y")
		{
			calc.SetVarValue("x", "2");
			calc.SetVarValue("y", "0");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('/', { "x", "y" })));

			THEN("fn = NAN")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("fn")));
			}
		}

		WHEN("x = nan; y = 0; fn = x + y")
		{
			calc.DefineVar("x");
			calc.SetVarValue("y", "0");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "x", "y" })));

			THEN("fn = NAN")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("fn")));
			}
		}

		WHEN("x = nan; fn = x")
		{
			calc.DefineVar("x");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(' ', { "x" })));

			THEN("fn = NAN")
			{
				REQUIRE(std::isnan(calc.GetIdentifierValue("fn")));
			}
		}

		WHEN("x = 1; fn = x")
		{
			calc.SetVarValue("x", "1");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>(' ', { "x" })));

			THEN("fn = 1")
			{
				REQUIRE(calc.GetIdentifierValue("fn") == 1);
			}
		}

		WHEN("x = 1; fn = x + x")
		{
			calc.SetVarValue("x", "1");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "x", "x" })));

			THEN("fn = 2")
			{
				REQUIRE(calc.GetIdentifierValue("fn") == 2);
			}
		}

		WHEN("x = 2; y = 3; z = 1; fn = x + y; fn1 = z * fn; fn2 = fn / fn1")
		{
			calc.SetVarValue("x", "2");
			calc.SetVarValue("z", "4");
			calc.SetVarValue("y", "3");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "x", "y" })));
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn1", std::pair<char, std::vector<std::string>>('*', { "z", "fn" })));
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn2", std::pair<char, std::vector<std::string>>('/', { "fn", "fn1" })));

			THEN("fn = 5; fn1 = 20; fn2 = 0.25")
			{
				REQUIRE(calc.GetIdentifierValue("fn") == 5);
				REQUIRE(calc.GetIdentifierValue("fn1") == 20);
				REQUIRE(calc.GetIdentifierValue("fn2") == 0.25);
			}
		}

		WHEN("x = nan; y = 0; fn = x + y; x = 1; y = 2")
		{
			calc.DefineVar("x");
			calc.SetVarValue("y", "0");
			calc.SetFunctionValue(std::pair<std::string, Calculator::Expression>("fn", std::pair<char, std::vector<std::string>>('+', { "x", "y" })));

			calc.SetVarValue("x", "1");
			calc.SetVarValue("y", "2");

			THEN("fn = 3")
			{
				REQUIRE(calc.GetIdentifierValue("fn") == 3);
			}
		}
	}
}
