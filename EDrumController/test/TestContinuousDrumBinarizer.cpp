#include "ContinuousDrumBinarizer.hpp"
#include "gtest/gtest.h"

// test file reading
#include <fstream>
#include <iostream>
#include <string>

class TestContinuousDrumBinarizer : public ::testing::Test
{
protected:
    ContinuousDrumBinarizer kick{KICK_IDLE_VALUE, KICK_MIDI_SIGNAL};
};

TEST_F(TestContinuousDrumBinarizer, getHitValue)
{
    EXPECT_EQ(kick.getHitVelocity(), MIDI_MAX_VALUE);
}
