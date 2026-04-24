project(parseurl)

add_executable(parseurl
        src/main.cpp
        src/ParseUrl.cpp
)

add_executable(parseurl_tests
        tests/tests.cpp
        src/ParseUrl.cpp
)

target_link_libraries(parseurl_tests PRIVATE Catch2::Catch2WithMain)
