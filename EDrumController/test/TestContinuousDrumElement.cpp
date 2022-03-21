#include "ContinuousDrumElement.hpp"
#include "gtest/gtest.h"

// test file reading
#include <fstream>
#include <iostream>
#include <string>

class TestContinuousDrumElement : public ::testing::Test
{
protected:
    void debounceToValue(uint16_t value)
    {
        for(auto i = 0; i < 200; i++)
        {
            crash.updateState(CRASH_MID_VAL);
        }
    }

    void readTestFile(std::string testFileName, std::vector<int>& data)
    {
        std::ifstream inFile;
        inFile.open(testFileName);
        std::string currentLine;

        if(inFile.is_open())
        {
            while(inFile)
            {
                std::getline(inFile, currentLine);
                data.push_back(std::stoi(currentLine));
            }

            inFile.close(); // CLose input file
        }
        else
        { //Error message
            std::cerr << "Can't find input file " << testFileName << std::endl;
        }
    }

    void checkNumberOfHits(std::vector<int>& indata, unsigned int expectedNumberOfHits)
    {
        volatile unsigned int hitCounter = 0;
        bool previousState = true;

        for(int i = 0; i < indata.size(); i++)
        {
            if(i == 4710)
            {
                std::cout << "Break!";
            }
            crash.updateState(indata.at(i));
            if(previousState == false && crash.wasDrumHit() == true)
            {
                hitCounter++;
            }
            previousState = crash.wasDrumHit();
        }

        EXPECT_EQ(hitCounter, expectedNumberOfHits);
    }

    ContinuousDrumElement crash{CRASH_MID_VAL, CRASH_SIGNAL};
    int acceptableOffset = 5U;
};

TEST_F(TestContinuousDrumElement, initialState)
{
    // drum hit is blocked at init
    EXPECT_EQ(crash.wasDrumHit(), true);
}

TEST_F(TestContinuousDrumElement, updatedState_exactCycles)
{
    debounceToValue(CRASH_MID_VAL);
    EXPECT_EQ(crash.wasDrumHit(), false);
}

TEST_F(TestContinuousDrumElement, NumberOfHits_3_FourTimes)
{
    std::vector<int> indata;
    readTestFile("../../TestData/3hits_ManyTimes_CymbalCrash_NoDelay.txt", indata);
    checkNumberOfHits(indata, 12U);
}

TEST_F(TestContinuousDrumElement, NumberOfHits_2_OneTime)
{
    std::vector<int> indata;
    readTestFile("../../TestData/2HitsTests_OneTime_CymbalCrash_NoDelay.txt", indata);
    checkNumberOfHits(indata, 2U);
}

TEST_F(TestContinuousDrumElement, NumberOfHits_2_FourTimes)
{
    std::vector<int> indata;
    readTestFile("../../TestData/2HitsTests_FourTimes_CymbalCrash_NoDelay.txt", indata);
    checkNumberOfHits(indata, 8U);
}

TEST_F(TestContinuousDrumElement, NumberOfHits_8_OneTime_Fast)
{
    std::vector<int> indata;
    readTestFile("../../TestData/8FastHitsTests_OneTime_CymbalCrash_NoDelay.txt", indata);
    checkNumberOfHits(indata, 8U);
}

TEST_F(TestContinuousDrumElement, NumberOfHits_5_OneTime_Fast)
{
    std::vector<int> indata;
    readTestFile("../../TestData/5HitsTests_OneTime_CymbalCrash_NoDelay.txt", indata);
    checkNumberOfHits(indata, 5U);
}

// value test getHitValue()
