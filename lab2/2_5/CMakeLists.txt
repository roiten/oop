project(htmldecode)

add_executable(htmldecode
        src/main.cpp
        src/decoder.h
        src/decoder.cpp
)

add_executable(decoder_tests
        tests/decoder_tests.cpp
        src/decoder.h
        src/decoder.cpp
        ../4_4/src/PrimeGen.h
        ../4_4/src/PrimeGen.cpp
)

target_link_libraries(decoder_tests PRIVATE Catch2::Catch2WithMain)
