project(1_3)

add_executable(1_3
        src/main.cpp
        src/findtext.cpp
)

add_executable(tests
        tests/check-substring.cpp
        tests/find-string.cpp
        src/findtext.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

target_include_directories(tests PRIVATE ${catch2_SOURCE_DIR}/single_include)

enable_testing()
add_test(NAME MyTests COMMAND tests)
