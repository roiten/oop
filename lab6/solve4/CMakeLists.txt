project(solve4)

add_executable(solve4 main.cpp)

add_library(solve4_lib INTERFACE)
target_include_directories(solve4_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)