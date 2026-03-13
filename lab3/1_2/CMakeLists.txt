project(car)

add_executable(car
        src/main.cpp
        src/Car.h
        src/Car.cpp
)


add_executable(car_tests
        tests/car_tests.cpp
        src/Car.h
        src/Car.cpp
)

target_link_libraries(car_tests PRIVATE Catch2::Catch2WithMain)
