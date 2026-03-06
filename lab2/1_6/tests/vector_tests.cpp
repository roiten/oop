#include <catch2/catch_test_macros.hpp>
#include "../src/TransformVector.h"

#include <iostream>

//тест с одинаковыми значениями

TEST_CASE("Прибавляет сумму трех минимальных")
{
    std::vector numbers = {5.0, 1.0, 4.0, 2.0, 3.0};
    double sum = 1.0 + 2.0 + 3.0;
    
    ProcessNumbers(numbers);
    
    REQUIRE(numbers[0] == 5.0 + sum);
    REQUIRE(numbers[1] == 1.0 + sum);
    REQUIRE(numbers[2] == 4.0 + sum);
    REQUIRE(numbers[3] == 2.0 + sum);
    REQUIRE(numbers[4] == 3.0 + sum);
}

TEST_CASE("Работает с отрицательными числами")
{
    std::vector numbers = {-5.0, -1.0, -4.0, -2.0, -3.0};
    double sum = -5.0 + -4.0 + -3.0;
    
    ProcessNumbers(numbers);
    
    REQUIRE(numbers[0] == -5.0 + sum);
    REQUIRE(numbers[1] == -1.0 + sum);
    REQUIRE(numbers[2] == -4.0 + sum);
    REQUIRE(numbers[3] == -2.0 + sum);
    REQUIRE(numbers[4] == -3.0 + sum);
}

TEST_CASE("обработка меньше трех чисел")
{
    std::vector numbers = {1.0, 2.0};
    double sum = 1.0 + 2.0;
    
    ProcessNumbers(numbers);
    
    REQUIRE(numbers[0] == 1.0 + sum);
    REQUIRE(numbers[1] == 2.0 + sum);
}

TEST_CASE("обработка 0 чисел")
{
	std::vector<double> numbers = {};
	ProcessNumbers(numbers);
	REQUIRE(numbers.empty());
}