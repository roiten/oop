project(primegen)

add_executable(primegen
        src/main.cpp
        src/PrimeGen.cpp
        src/PrimeGen.h
)

add_executable(primegen_tests
        tests/primegen_tests.cpp
        src/PrimeGen.cpp
        src/PrimeGen.h
)

target_link_libraries(primegen_tests PRIVATE Catch2::Catch2WithMain)
