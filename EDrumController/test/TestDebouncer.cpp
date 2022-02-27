#include "Debouncer.hpp"
#include "gtest/gtest.h"

const uint8_t testValue = 10U;
const uint8_t debounceCycles = 5U;

TEST(TestDebouncer, initialState)
{
    Debouncer<uint8_t> debounce(testValue);
    EXPECT_EQ(debounce.getState(), DebounceState::Debounce);
}

TEST(TestDebouncer, updatedState_exactCycles)
{
    Debouncer<uint8_t> debounce(debounceCycles);
    for(auto i = 0; i <= debounceCycles; i++)
    {
        debounce.updateState(testValue);
    }
    EXPECT_EQ(debounce.getState(), DebounceState::Stable);
}

TEST(TestDebouncer, updatedState_moreCycles)
{
    Debouncer<uint8_t> debounce(debounceCycles);
    for(auto i = 0; i < debounceCycles * 2; i++)
    {
        debounce.updateState(testValue);
    }
    EXPECT_EQ(debounce.getState(), DebounceState::Stable);
}

TEST(TestDebouncer, updatedState_backToDebounce)
{
    Debouncer<uint8_t> debounce(debounceCycles);
    for(auto i = 0; i < debounceCycles * 2; i++)
    {
        debounce.updateState(testValue);
    }
    debounce.updateState(testValue * 2);
    EXPECT_EQ(debounce.getState(), DebounceState::Debounce);
}
