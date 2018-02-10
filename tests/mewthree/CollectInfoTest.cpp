/**
 * CollectInfoTest.cpp
 */
extern "C"
{
#include "common/types.h"
#include "common/strres.h"
#include "common/Observer.h"
#include "common/ReadWrite.h"
#include "file/File.h"
#include "file/RomFile.h"
#include "smw/libsmw.h"
#include "mewthree/Signature.h"
#include "mewthree/InsInfo.h"
#include "mewthree/CollectInfo.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(CollectInfo)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(CollectInfo, Create)
{
	FAIL("Start here");
}

