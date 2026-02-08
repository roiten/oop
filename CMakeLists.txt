cmake_minimum_required(VERSION 3.14)
project(Labs)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

add_subdirectory(lab1)
