#include "BinaryElement.hpp"
#include "gtest/gtest.h"

// test file reading
#include <fstream>
#include <iostream>
#include <string>

class TestBinaryDrumElement : public ::testing::Test
{
protected:
    void checkNumberOfHits(std::vector<int>& indata,
                           unsigned int expectedNumberOfHits,
                           BinaryDrumElement& drum)
    {
        volatile unsigned int hitCounter = 0;
        bool previousState = true;

        for(auto i = 0U; i < indata.size(); i++)
        {
            if(i == 24089)
            {
                volatile int a = 0;
            }
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

    std::string testdataPath = "D:/Workspace/EDrums/TestData/";
    BinaryDrumElement crashChoke{1U, CHOKE_SIGNAL};
};

TEST_F(TestBinaryDrumElement, initialState)
{
    // drum hit is blocked at init
    EXPECT_EQ(crashChoke.wasDrumHit(), true);
}

TEST_F(TestBinaryDrumElement, ChokeCrash_6ShortTimes)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("Choke_6times.txt"), indata);
    checkNumberOfHits(indata, 6U, crashChoke);
}

TEST_F(TestBinaryDrumElement, ChokeCrash_2LongTimes)
{
    std::vector<int> indata;
    readTestFile(testdataPath.append("Choke_2longtimes.txt"), indata);
    checkNumberOfHits(indata, 2U, crashChoke);
}
