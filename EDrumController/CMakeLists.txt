# add the executable
add_executable(drumtests 
    test/TestDrumElement.cpp
    test/TestDebouncer.cpp
    test/TestFilter.cpp
    src/DrumController/DrumController.cpp
    src/DrumElement/ContinuousDrumElement.cpp
)

# add includes
add_subdirectory(test/googletest)
include_directories(
    googletest/include
    src/Utils
    src/DrumController
    src/DrumElement
    src/Other
)

# target libraries
target_link_libraries(drumtests PRIVATE gtest)
