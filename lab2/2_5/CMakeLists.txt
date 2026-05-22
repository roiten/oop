project(htmldecode)

add_executable(htmldecode
        src/main.cpp
        src/Decoder.h
        src/Decoder.cpp
)

add_executable(decoder_tests
        tests/decoder_tests.cpp
        src/Decoder.h
        src/Decoder.cpp
)

target_link_libraries(decoder_tests PRIVATE Catch2::Catch2WithMain)
