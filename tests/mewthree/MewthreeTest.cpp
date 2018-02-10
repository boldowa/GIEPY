/**
 * MewthreeTest.cpp
 */
#include <assert.h>
extern "C"
{
#include "common/types.h"
#include "mewthree/Mewthree.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(Mewthree)
{
	/* test target */
	Mewthree* target;

	void setup()
	{
		target = new_Mewthree();
	}

	void teardown()
	{
		delete_Mewthree(&target);
	}
};

/**
 * Check object create
 */
TEST(Mewthree, new)
{
	CHECK(NULL != target);

	FAIL("Starts here.");
}

/**
 * Check object delete
 */
TEST(Mewthree, delete)
{
	delete_Mewthree(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}

