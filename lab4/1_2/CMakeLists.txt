project(shapes)

add_executable(shapes
        src/main.cpp

        src/shapes/CCircle.h
        src/shapes/CLineSegment.h
        src/shapes/CRectangle.h
        src/shapes/CTriangle.h
        src/shapes/IShape.h
        src/shapes/ISolidShape.h
        src/shapes/Point.h
)


add_executable(shapes_tests
        tests/shapes_tests.cpp

        src/shapes/CCircle.h
        src/shapes/CLineSegment.h
        src/shapes/CRectangle.h
        src/shapes/CTriangle.h
        src/shapes/IShape.h
        src/shapes/ISolidShape.h
        src/shapes/Point.h
)

target_link_libraries(car_tests PRIVATE Catch2::Catch2WithMain)
