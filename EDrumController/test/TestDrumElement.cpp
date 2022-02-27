#include "DrumController.hpp"
#include "gtest/gtest.h"

#include <fstream>
#include <string>

TEST(TestContinuousDrumElement, DummyTest)
{
    DrumController controller;
    volatile int a = 0;
    controller.cycle();
    EXPECT_EQ(a, 0);
}

TEST(TestContinuousDrumElement, DrumWasNotHit)
{
    // TODO - signal in the idle state
}

TEST(TestContinuousDrumElement, DrumWasHit)
{
    // TODO - hit was received
}

TEST(TestContinuousDrumElement, ShortDisturbance)
{
    // TODO - bigger value only for 1 or 2 cycles
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
