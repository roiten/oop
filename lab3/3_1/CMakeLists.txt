project(bank)

file(GLOB SOURCES
        src/Actors/Apu/Apu.cpp
        src/Actors/Bank/Bank.cpp
        src/Actors/Bart/Bart.cpp
        src/Actors/Berns/Berns.cpp
        src/Actors/Contact/ContactList.cpp
        src/Actors/Homer/Homer.cpp
        src/Actors/Lisa/Lisa.cpp
        src/Actors/Marge/Marge.cpp
        src/Actors/Person/Person.cpp
        src/Actors/PersonWithAccount/PersonWithAccount.cpp
        src/Simulation/Simulation.cpp
        src/utils.cpp
)

add_executable(start ${SOURCES} src/main.cpp)

#add_executable(calculator_tests
#        tests/calculator_tests.cpp
##        tests/controller_tests.cpp
#)

#target_link_libraries(calculator_tests PRIVATE Catch2::Catch2WithMain)
