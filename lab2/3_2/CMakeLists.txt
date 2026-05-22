project(dict)

add_executable(dict
        src/main.cpp
        src/Dict.h
        src/Dict.cpp
        src/InputSpread.h
)

add_executable(dict_tests
        tests/dict_tests.cpp
        src/Dict.h
        src/Dict.cpp
)

target_link_libraries(dict_tests PRIVATE Catch2::Catch2WithMain)
