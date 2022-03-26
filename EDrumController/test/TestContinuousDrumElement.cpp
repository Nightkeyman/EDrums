#include "ContinuousDrumElement.hpp"
#include "gtest/gtest.h"

// test file reading
#include <fstream>
#include <iostream>
#include <string>

class TestContinuousDrumElement : public ::testing::Test
{
protected:
    void debounceToValue(ContinuousDrumElement& drum, uint16_t idleState)
    {
        for(auto i = 0; i < 200; i++)
        {
            drum.updateState(idleState);
        }
    }

    void checkNumberOfHits(std::vector<int>& indata,
                           unsigned int expectedNumberOfHits,
                           ContinuousDrumElement& drum)
    {
        volatile unsigned int hitCounter = 0;
        bool previousState = true;

        for(auto i = 0U; i < indata.size(); i++)
        {
            drum.updateState(indata.at(i));
            if((previousState == false) && (drum.wasDrumHit() == true))
            {
                hitCounter++;
                std::cout << i << " " << std::endl;
            }
            previousState = drum.wasDrumHit();
        }

        EXPECT_EQ(hitCounter, expectedNumberOfHits);
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

    int acceptableOffset = 5U;
    std::string testdataPath = "D:/Workspace/EDrums/TestData/";
};

class TestCrashCymbal : public TestContinuousDrumElement
{
protected:
    ContinuousDrumElement crash{CRASH_MID_VAL, CRASH_SIGNAL};
};

TEST_F(TestCrashCymbal, initialState)
{
    // drum hit is blocked at init
    EXPECT_EQ(crash.wasDrumHit(), true);
}

TEST_F(TestCrashCymbal, updatedState_exactCycles)
{
    debounceToValue(crash, CRASH_MID_VAL);
    EXPECT_EQ(crash.wasDrumHit(), false);
}

TEST_F(TestCrashCymbal, Crash_NumberOfHits_3_FourTimes)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("3hits_ManyTimes_CymbalCrash_NoDelay.txt"), indata);
    checkNumberOfHits(indata, 12U, crash);
}

TEST_F(TestCrashCymbal, Crash_NumberOfHits_2_OneTime)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("2HitsTests_OneTime_CymbalCrash_NoDelay.txt"), indata);
    checkNumberOfHits(indata, 2U, crash);
}

TEST_F(TestCrashCymbal, Crash_NumberOfHits_2_FourTimes)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("2HitsTests_FourTimes_CymbalCrash_NoDelay.txt"), indata);
    checkNumberOfHits(indata, 8U, crash);
}

TEST_F(TestCrashCymbal, Crash_NumberOfHits_8_OneTime_Fast)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("8FastHitsTests_OneTime_CymbalCrash_NoDelay.txt"), indata);
    checkNumberOfHits(indata, 8U, crash);
}

TEST_F(TestCrashCymbal, Crash_NumberOfHits_5_OneTime_Fast)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("5HitsTests_OneTime_CymbalCrash_NoDelay.txt"), indata);
    checkNumberOfHits(indata, 5U, crash);
}

TEST_F(TestCrashCymbal, Crash_NumberOfHits_ManyStrongHits)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("ManyStrongHits.txt"), indata);
    checkNumberOfHits(indata, 10U, crash);
}

TEST_F(TestCrashCymbal, Crash_NumberOfHits_ManyDelicateHits)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("ManyDelicateHits.txt"), indata);
    checkNumberOfHits(indata, 46U, crash);
}

class TestTomDrum : public TestContinuousDrumElement
{
protected:
    const uint16_t testIdleValue = 383U;
    ContinuousDrumElement tom{testIdleValue, KICK_MIDI_SIGNAL};
};

TEST_F(TestTomDrum, UpdatedState_exactCycles)
{
    debounceToValue(tom, testIdleValue);
    EXPECT_EQ(tom.wasDrumHit(), false);
}

TEST_F(TestTomDrum, Tom_NumberOfHits_9Hits)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("tom_9hits_NoDelay.txt"), indata);
    checkNumberOfHits(indata, 9U, tom);
}

// TODO: Test value test getHitValue()
TEST_F(TestTomDrum, InitialState_Value)
{
    std::vector<int> indata;
    debounceToValue(tom, testIdleValue);

    EXPECT_EQ(tom.getHitVelocity(), 0U);
}
