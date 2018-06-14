/**
 * PipeManTest.cpp
 */
#include <assert.h>
#include <bolib.h>
#include "common/PipeMan.h"

#include "CppUTest/TestHarness.h"

TEST_GROUP(PipeMan)
{
	/* test target */
	PipeMan* target;

	void setup()
	{
		target = new_PipeMan("echo a");
	}

	void teardown()
	{
		delete_PipeMan(&target);
	}
};

/**
 * Check object create
 */
TEST(PipeMan, new)
{
	CHECK(NULL != target);

	FAIL("Starts here.");
}

/**
 * Check object delete
 */
TEST(PipeMan, delete)
{
	delete_PipeMan(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}

