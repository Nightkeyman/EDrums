#include "ContinuousDrumBinarizer.hpp"
#include "gtest/gtest.h"

// test file reading
#include <fstream>
#include <iostream>
#include <string>

class TestContinuousDrumBinarizer : public ::testing::Test
{
protected:
    void debounceToValue(ContinuousDrumBinarizer& drum, uint16_t idleState)
    {
        for(auto i = 0; i < 200; i++)
        {
            drum.updateState(idleState);
        }
    }

    void checkNumberOfHits(std::vector<int>& indata,
                           unsigned int expectedNumberOfHits,
                           ContinuousDrumBinarizer& drum)
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

    ContinuousDrumBinarizer kick{KICK_IDLE_VALUE, KICK_MIDI_SIGNAL, 1U};
    std::string testdataPath = "D:/Workspace/EDrums/TestData/";
};

TEST_F(TestContinuousDrumBinarizer, getHitValue)
{
    EXPECT_EQ(kick.getHitVelocity(), MIDI_MAX_VALUE);
}

TEST_F(TestContinuousDrumBinarizer, tomHits_10)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("tom_withStick.txt"), indata);
    checkNumberOfHits(indata, 10U, kick);
}
