add_executable(solve4_tests solve4_test.cpp)

target_link_libraries(solve4_tests PRIVATE
        Catch2::Catch2WithMain
        solve4_lib
)