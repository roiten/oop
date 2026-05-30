add_executable(ListTests
        ConstructorTests.cpp
        ExceptionsAndBoundaryTests.cpp
        IteratorTests.cpp
        MemberTypeTests.cpp
        ModificationTests.cpp
)

target_link_libraries(ListTests PRIVATE Catch2::Catch2WithMain)