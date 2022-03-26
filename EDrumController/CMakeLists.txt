# add the executable
add_executable(drums_unittest
    test/TestDrumElement.cpp
    test/TestDebouncer.cpp
    test/TestFilter.cpp
    test/TestContinuousDrumElement.cpp
    test/TestContinuousDrumBinarizer.cpp
    src/DrumController/DrumController.cpp
    src/DrumElement/ContinuousDrumElement.cpp
    src/DrumElement/BinaryElement.cpp
    src/DrumElement/ContinuousDrumBinarizer.cpp
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
target_link_libraries(drums_unittest PRIVATE gtest)
