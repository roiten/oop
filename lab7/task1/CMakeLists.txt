project(sort2)

add_executable(sort2
        src/main.cpp
)


add_executable(sort2_tests
        tests/sort2_tests.cpp
)

target_link_libraries(sort2_tests PRIVATE Catch2::Catch2WithMain)
