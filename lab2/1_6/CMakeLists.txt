project(vector)

add_executable(vector
        src/main.cpp
        src/vector.h
        src/vector.cpp
)


add_executable(vector_tests
        tests/vector_tests.cpp
        src/vector.h
        src/vector.cpp
)

target_link_libraries(vector_tests PRIVATE Catch2::Catch2WithMain)
