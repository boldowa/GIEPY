/**
 * MewthreeDllTest.cpp
 */
extern "C"
{
#include "common/types.h"
#include "common/Environment.h"
#include "common/Observer.h"
}

#include "dll/MewthreeDll.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(LoadMewthree)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(LoadMewthree, Create)
{
  FAIL("Start here");
}

