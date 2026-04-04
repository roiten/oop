project(parseurl)

add_executable(parseurl
        src/main.cpp
        src/ParseUrl.h
        src/ParseUrl.cpp
)

#add_executable(primegen_tests
#        tests/primegen_tests.cpp
#        src/PrimeGen.cpp
#        src/PrimeGen.h
#)

#target_link_libraries(parseurl_tests PRIVATE Catch2::Catch2WithMain)
