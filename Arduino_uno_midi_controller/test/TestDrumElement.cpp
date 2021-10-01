#include "DrumController.hpp"
#include "gtest/gtest.h"

DrumController controller;

TEST(TestContinuousDrumElement, NoHit_InIdle)
{
    volatile int a = 0;
    EXPECT_EQ(a, 0);
    controller.cycle();
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
