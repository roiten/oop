cmake_minimum_required(VERSION 3.20)
project(shapes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shapes_visualizer
        src/main.cpp
        src/shapes/CCircle.cpp
        src/shapes/CLineSegment.cpp
        src/shapes/CRectangle.cpp
        src/shapes/CTriangle.cpp
        src/canvas/CCanvas.cpp
        src/shapes/FindProperties.cpp
)

target_link_libraries(shapes_visualizer
        sfml-graphics
        sfml-window
        sfml-system
)

target_include_directories(shapes_visualizer PRIVATE src)

add_executable(shapes_visualizer_tests
        tests/shapes_tests.cpp

        src/shapes/CCircle.cpp
        src/shapes/CLineSegment.cpp
        src/shapes/CRectangle.cpp
        src/shapes/CTriangle.cpp
        src/canvas/CCanvas.cpp
        src/shapes/FindProperties.cpp

)

target_link_libraries(shapes_visualizer_tests
        Catch2::Catch2WithMain
        sfml-graphics
        sfml-window
        sfml-system
)

target_include_directories(shapes_visualizer_tests PRIVATE
        src
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(shapes_draw_tests
        tests/draw_tests.cpp

        src/shapes/CCircle.cpp
        src/shapes/CLineSegment.cpp
        src/shapes/CRectangle.cpp
        src/shapes/CTriangle.cpp
        src/canvas/CCanvas.cpp
        src/shapes/FindProperties.cpp

)

target_link_libraries(shapes_draw_tests
        Catch2::Catch2WithMain
        sfml-graphics
        sfml-window
        sfml-system
)

target_include_directories(shapes_draw_tests PRIVATE
        src
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${fakeit_SOURCE_DIR}/single_header/catch
        ${fakeit_SOURCE_DIR}/include
)

if(WIN32)
    target_link_libraries(shapes_visualizer
            opengl32
            winmm
            gdi32
            ws2_32
    )
    target_link_libraries(shapes_visualizer_tests
            opengl32
            winmm
            gdi32
            ws2_32
    )
    target_link_libraries(shapes_draw_tests
            opengl32
            winmm
            gdi32
            ws2_32
    )
endif()