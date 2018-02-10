extern "C"
{
#include "common/types.h"
#include <string.h>
#include "common/Str.h"
#include "common/Environment.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(Environment)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(Environment, SetEnvironment)
{
	SetSystemEnvironment();
	CHECK(NULL != Environment.ExeDir);
	CHECK(NULL != Environment.CurDir);
}

TEST(Environment, SetSearchPath)
{
	strcpy_s((char*)Environment.ExeDir, MAX_PATH, "./tests/");
	strcpy_s((char*)Environment.CurDir, MAX_PATH, "./testdata/");
	Environment.RomDir = Str_copy("./testdata/");

	SetSearchPath();

	POINTERS_EQUAL(NULL, Environment.SearchPath[2]);

	strcpy_s((char*)Environment.ExeDir, MAX_PATH, "./testdata/");
	strcpy_s((char*)Environment.CurDir, MAX_PATH, "./tests/");

	SetSearchPath();

	POINTERS_EQUAL(NULL, Environment.SearchPath[2]);

	free((char*)Environment.RomDir);
	Environment.RomDir = NULL;
}

