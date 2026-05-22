project(templates_task1)

add_executable(findmax
        src/main.cpp
)

add_executable(sort2_tests
        tests/sort2_tests.cpp
)

add_executable(findmax_tests
        tests/findmax_tests.cpp
)
add_executable(findmaxex_tests
        tests/findmaxex_tests.cpp
)

target_link_libraries(sort2_tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(findmax_tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(findmaxex_tests PRIVATE Catch2::Catch2WithMain)
