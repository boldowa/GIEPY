/**
 * LibsInsertManTest.cpp
 */
#include <assert.h>
extern "C"
{
#include "common/types.h"
#include "common/Str.h"
#include "unko/LibsInsertMan.h"
#include "common/Enviroment.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(LibsInsertMan)
{
	/* test target */
	LibsInsertMan* target;

	void setup()
	{
		for(int i=0; i<SearchPathNums; i++)
		{
			Enviroment.SearchPath[i] = NULL;
		}

		target = new_LibsInsertMan();
	}

	void teardown()
	{
		delete_LibsInsertMan(&target);
	}
};

/**
 * Check object create
 */
TEST(LibsInsertMan, new)
{
	CHECK(NULL != target);
	LONGS_EQUAL(0, target->filesCount(target));
	LONGS_EQUAL(0, target->labelsCount(target));
}

/**
 * Check object delete
 */
TEST(LibsInsertMan, delete)
{
	delete_LibsInsertMan(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}

/**
 * Check filesCount method
 */
TEST(LibsInsertMan, filesCount)
{
	/* nothibg to do here */
}

/**
 * Check labelsCount method
 */
TEST(LibsInsertMan, labelsCount)
{
	/* nothibg to do here */
}

/**
 * Check buikdData method
 */
TEST(LibsInsertMan, buildData)
{
	Enviroment.SearchPath[0] = "./testdata/unko/";

	target->buildData(target, "lib1");

	/**
	 * Check label search feature
	 *   lib1/testlib1.asm  : 2 labels
	 *   lib1/testlib2.asm  : 2 labels, but it's same as testlib1.asm
	 *                        so, it'll not be counted.
	 *   lib1/comlib.asm    : 13 labels.
	 */
	LONGS_EQUAL(3, target->filesCount(target));
	LONGS_EQUAL(15, target->labelsCount(target));

	/*
	printf("\nfiles : %ld", target->filesCount(target));
	printf("\nlabels: %ld\n", target->labelsCount(target));
	*/

	/**********************************************************/

	/**
	 * Check search feature
	 *   ... label not found case
	 */
	POINTERS_EQUAL(NULL, target->searchLibrary(target, "UNKO"));
	/**
	 *   ... label found case
	 */
	LibraryFileItem* item = target->searchLibrary(target, "RightSet");
	CHECK(NULL != item);
	STRCMP_EQUAL("./testdata/unko/lib1/comlib.asm", item->libpath);
	CHECK_FALSE(item->isInserted);

	/**
	 * inserted detection check
	 */
	item->isInserted = true;
	item = target->searchLibrary(target, "DownSet");  // search another label in same file
	CHECK(NULL != item);
	STRCMP_EQUAL("./testdata/unko/lib1/comlib.asm", item->libpath);
	CHECK(item->isInserted);

	item = target->searchLibrary(target, "Lib1_XXX");  // search label in another file
	CHECK(NULL != item);
	CHECK_FALSE(item->isInserted);
}

/**
 * Check searchLibrary method
 */
TEST(LibsInsertMan, searchLibrary)
{
	/* tested in buildData , so nothing to do here */
}

