#include "IIRFilter.hpp"
#include "gtest/gtest.h"

const float possibleError = 0.05F;
const float smallError = 0.009F;

TEST(TestFilter, SmallFilterFactor)
{
    const float filterFactor = 0.1F;
    const float setState = 1.0F;
    IIRFilter _filter(filterFactor);

    EXPECT_NEAR(_filter.getUpdatedOutput(setState), 0.1F, possibleError);
    EXPECT_NEAR(_filter.getUpdatedOutput(setState), 0.2F, possibleError);
    EXPECT_NEAR(_filter.getUpdatedOutput(setState), 0.27F, possibleError);
}

TEST(TestFilter, BigFilterFactor)
{
    const float filterFactor = 0.5F;
    const float setState = 1.0F;
    IIRFilter _filter(filterFactor);

    EXPECT_NEAR(_filter.getUpdatedOutput(setState), 0.5F, possibleError);
    EXPECT_NEAR(_filter.getUpdatedOutput(setState), 0.75F, possibleError);
    EXPECT_NEAR(_filter.getUpdatedOutput(setState), 0.9F, possibleError);
}

TEST(TestFilter, ReachingFinalState)
{
    const float filterFactor = 0.1F;
    const float setState = 1.0F;
    const int filteringCycles = 50;
    IIRFilter _filter(filterFactor);

    for(auto i = 0U; i < filteringCycles; i++)
    {
        _filter.getUpdatedOutput(setState);
    }

    EXPECT_NEAR(_filter.getUpdatedOutput(setState), setState, smallError);
}
