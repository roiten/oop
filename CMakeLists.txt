cmake_minimum_required(VERSION 3.14)
project(Labs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_compile_options(/utf-8)
endif()

include(FetchContent)

#FetchContent_Declare(
#        SFML
#        GIT_REPOSITORY https://github.com/SFML/SFML.git
#        GIT_TAG 2.6.1
#        GIT_SHALLOW ON
#        SYSTEM
#)

#set(BUILD_SHARED_LIBS OFF)
#set(SFML_BUILD_EXAMPLES OFF)
#set(SFML_BUILD_DOC OFF)
#set(SFML_BUILD_NETWORK OFF)
#set(SFML_BUILD_AUDIO OFF)
#set(SFML_BUILD_GRAPHICS ON)
#set(SFML_BUILD_WINDOW ON)
#
#FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)


#FetchContent_Declare(
#        FakeIt
#        GIT_REPOSITORY https://github.com/eranpeer/FakeIt.git
#        GIT_TAG master
#)
#FetchContent_MakeAvailable(FakeIt)

#add_subdirectory(lab2)
#add_subdirectory(lab3)
#add_subdirectory(lab4)
add_subdirectory(lab5)
add_subdirectory(lab6)