/**
 * IniManTest.cpp
 */
#include <assert.h>
#include <bolib.h>
#include "win32/IniMan.h"

#include "CppUTest/TestHarness.h"

TEST_GROUP(IniMan)
{
	/* test target */
	IniMan* target;

	void setup()
	{
		target = new_IniMan();
	}

	void teardown()
	{
		delete_IniMan(&target);
	}
};

/**
 * Check object create
 */
TEST(IniMan, new)
{
	CHECK(NULL != target);

	FAIL("Starts here.");
}

/**
 * Check object delete
 */
TEST(IniMan, delete)
{
	delete_IniMan(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}

