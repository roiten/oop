project(dict)

add_executable(dict
        src/main.cpp
        src/dict.h
        src/dict.cpp
        src/InputSpread.h
)

add_executable(dict_tests
        tests/dict_tests.cpp
        src/dict.h
        src/dict.cpp
)

target_link_libraries(dict_tests PRIVATE Catch2::Catch2WithMain)
