# add the executable
add_executable(drumtests 
    test/TestDrumElement.cpp
    src/DrumController/DrumController.cpp
    src/DrumElement/ContinuousDrumElement.cpp
)

# add includes
add_subdirectory(test/googletest)
include_directories(
    googletest/include
    src/DrumController
    src/DrumElement
    src/Debouncer
)

# target libraries
target_link_libraries(drumtests PRIVATE gtest)
