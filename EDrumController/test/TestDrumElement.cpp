#include "DrumController.hpp"
#include "gtest/gtest.h"

#include <fstream>
#include <string>

TEST(TestDrumController, DummyTest)
{
    // dummy test
    DrumController controller;
    volatile int a = 0;
    controller.cycle();
    EXPECT_EQ(a, 0);
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
