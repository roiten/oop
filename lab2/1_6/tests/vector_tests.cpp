#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "../src/vector.h"

#include <iostream>

std::streambuf* origCin = std::cin.rdbuf();
std::streambuf* origCout = std::cout.rdbuf();

TEST_CASE("Читаем несколько строчек")
{
    std::vector<double> numbers;
    std::istringstream input("1.5 2.715 3.14\n4.2 5.001\n\n");
    std::cin.rdbuf(input.rdbuf());
    
    ReadNumbers(numbers);
    
    REQUIRE(numbers.size() == 5);
    REQUIRE(numbers[0] == 1.5);
    REQUIRE(numbers[1] == 2.715);
    REQUIRE(numbers[2] == 3.14);
    REQUIRE(numbers[3] == 4.2);
    REQUIRE(numbers[4] == 5.001);
    
    std::cin.rdbuf(origCin);
}

TEST_CASE("Пустая строка останавливает чтение")
{
    std::vector<double> numbers;
    std::istringstream input("1 2 3\n\n4 5 6\n");
    std::cin.rdbuf(input.rdbuf());
    
    ReadNumbers(numbers);
    
    REQUIRE(numbers.size() == 3);
    REQUIRE(numbers[0] == 1);
    REQUIRE(numbers[1] == 2);
    REQUIRE(numbers[2] == 3);
    
    std::cin.rdbuf(origCin);
}

TEST_CASE("Кидает исключение при некорректных данных")
{
    std::vector<double> numbers;
    std::istringstream input("3 2 qwe 1\n");
    std::cin.rdbuf(input.rdbuf());
    
    REQUIRE_THROWS_AS(ReadNumbers(numbers), std::exception);
    
    std::cin.rdbuf(origCin);
}

TEST_CASE("Прибавляет сумму трех минимальных")
{
    std::vector<double> numbers = {5.0, 1.0, 4.0, 2.0, 3.0};
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
    std::vector<double> numbers = {-5.0, -1.0, -4.0, -2.0, -3.0};
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
    std::vector<double> numbers = {1.0, 2.0};
    double sum = 1.0 + 2.0;
    
    ProcessNumbers(numbers);
    
    REQUIRE(numbers[0] == 1.0 + sum);
    REQUIRE(numbers[1] == 2.0 + sum);
}

TEST_CASE("Проверка точности в 3 знака")
{
    std::vector<double> numbers = {1.23456, 2.34567, 3.45678};
    std::ostringstream output;
    std::cout.rdbuf(output.rdbuf());
    
    PrintSortedNumbers(numbers);
    
    std::string expected = "1.235 2.346 3.457 \n";
    REQUIRE(output.str() == expected);
    
    std::cout.rdbuf(origCout);
}

TEST_CASE("Подаём пустой вектор")
{
    std::vector<double> numbers;
    std::ostringstream output;
    std::cout.rdbuf(output.rdbuf());
    
    PrintSortedNumbers(numbers);
    
    REQUIRE(output.str() == "\n");
    
    std::cout.rdbuf(origCout);
}

TEST_CASE("Проверка работы точки входа программы")
{
    std::vector<double> numbers;
    std::istringstream input("-1.5 -2 -3.5\n-4.2 -5.0\n\n");
    std::cin.rdbuf(input.rdbuf());
    
    std::ostringstream output;
    std::cout.rdbuf(output.rdbuf());
    
    RunApp(numbers);

    std::string expected = "-17.700 -16.900 -16.200 -14.700 -14.200 \n";
    REQUIRE(output.str() == expected);
    
    std::cin.rdbuf(origCin);
    std::cout.rdbuf(origCout);
}