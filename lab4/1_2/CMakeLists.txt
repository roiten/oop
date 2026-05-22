project(shapes)

add_executable(shapes
        src/main.cpp

        src/shapes/CCircle.cpp
        src/shapes/CLineSegment.cpp
        src/shapes/CRectangle.cpp
        src/shapes/CTriangle.cpp
)


add_executable(shapes_tests
        tests/shapes_tests.cpp

        src/shapes/CCircle.cpp
        src/shapes/CLineSegment.cpp
        src/shapes/CRectangle.cpp
        src/shapes/CTriangle.cpp
)

target_link_libraries(shapes_tests PRIVATE Catch2::Catch2WithMain)
