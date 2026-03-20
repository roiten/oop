project(car)

add_executable(car
        src/main.cpp
        src/Car.h
        src/Car.cpp
        ../2_1/src/Calculator.h
        ../2_1/src/Calculator.cpp
        ../2_1/src/Controller.cpp
)


add_executable(car_tests
        tests/car_tests.cpp
        src/Car.h
        src/Car.cpp
)

target_link_libraries(car_tests PRIVATE Catch2::Catch2WithMain)
