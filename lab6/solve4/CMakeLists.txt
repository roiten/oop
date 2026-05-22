project(solve4)

add_library(solve4_lib STATIC solve4/solve4.cpp)

target_include_directories(solve4_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(solve4 main.cpp)
target_link_libraries(solve4 solve4_lib)

add_subdirectory(tests)