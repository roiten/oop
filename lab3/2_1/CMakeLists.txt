project(calculator)

add_executable(calculator
        src/main.cpp
        src/Calculator.h
        src/Calculator.cpp
        src/Controller.h
        src/Controller.cpp
)


add_executable(calculator_tests
        tests/calculator_tests.cpp
#        tests/controller_tests.cpp
        src/Calculator.h
        src/Calculator.cpp
)

target_link_libraries(calculator_tests PRIVATE Catch2::Catch2WithMain)
