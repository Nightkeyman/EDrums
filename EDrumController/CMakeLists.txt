# add the executable
add_executable(drumtests 
    test/TestDrumElement.cpp
    test/TestDebouncer.cpp
    test/TestFilter.cpp
    test/TestContinuousDrumElement.cpp
    src/DrumController/DrumController.cpp
    src/DrumElement/ContinuousDrumElement.cpp
    # Workaround for providing dummy "Serial" definition
    test/helpers/FakeSerial.cpp
)

add_definitions(-DGTEST_BUILD=1)
# add includes
add_subdirectory(test/googletest)
include_directories(
    googletest/include
    src/Utils
    src/DrumController
    src/DrumElement
    src/Other
    test/helpers
)

# target libraries
target_link_libraries(drumtests PRIVATE gtest)
