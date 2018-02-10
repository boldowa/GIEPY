extern "C"
{
#include <stdlib.h>
#include "puts.h"
#include "Option.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(Option)
{
	void setup()
	{
		/* nothing to do */
	}

	void teardown()
	{
		/* nothing to do */
	}
};

TEST(Option, ParseString)
{
	char* st = NULL;
	const char* argvs[] = {
		"dummy",
		"-s",
		"aaaaa",
		"file"
	};
	const char** argv = argvs;
	int argc = (sizeof(argvs)/sizeof(char*));
	OptionStruct op[] = {
		{ "st", 's', "desc", OptionType_String, &st },
		{ NULL, '\0', NULL, OptionType_Term, NULL }
	};
	LONGS_EQUAL(4, argc);
	CHECK(Option_Parse(&argc, (char***)&argv, op));
	STRCMP_EQUAL("aaaaa", *(char**)op[0].val);
	LONGS_EQUAL(2, argc);
}

TEST(Option, ParseBool)
{
	bool bl = false;
	const char* argvs[] = {
		"dummy",
		"-b",
	};
	const char* argvs2[] = {
		"dummy",
		"-bool",
	};
	const char** argv = argvs;
	const char** argv2 = argvs;
	int argc = (sizeof(argvs)/sizeof(char*));
	int argc2 = (sizeof(argvs2)/sizeof(char*));
	OptionStruct op[] = {
		{ "bool", 'b', "desc", OptionType_Bool, &bl },
		{ NULL, '\0', NULL, OptionType_Term, NULL }
	};
	CHECK(true != *(bool*)op[0].val);

	CHECK(Option_Parse(&argc, (char***)&argv, op));
	CHECK(true == *(bool*)op[0].val);
	/* toggle */
	CHECK(Option_Parse(&argc2, (char***)&argv2, op));
	CHECK(false == *(bool*)op[0].val);
}

TEST(Option, ParseInt)
{
	int i = 0;
	const char* argvs[] = {
		"dummy",
		"-i",
		"100",
		"file"
	};
	const char* argv2[] = {
		"dummy",
		"--int",
		"50a"
	};
	const char** argv = argvs;
	int argc = (sizeof(argvs)/sizeof(char*));
	OptionStruct op[] = {
		{ "int", 'i', "desc", OptionType_Int, &i },
		{ NULL, '\0', NULL, OptionType_Term, NULL }
	};
	LONGS_EQUAL(4, argc);
	CHECK(Option_Parse(&argc, (char***)&argv, op));
	LONGS_EQUAL(100, i);
	LONGS_EQUAL(2, argc);

	argv = argv2;
	argc = (sizeof(argv2)/sizeof(char*));
	CHECK_FALSE(Option_Parse(&argc, (char***)&argv, op));
	LONGS_EQUAL(3, argc);
}

TEST(Option, ParseFloat)
{
	float f = 0;
	const char* argvs[] = {
		"dummy",
		"-f",
		"100.5",
		"file"
	};
	const char* argv2[] = {
		"dummy",
		"--float",
		"3.12.4"
	};
	const char** argv = argvs;
	int argc = (sizeof(argvs)/sizeof(char*));
	OptionStruct op[] = {
		{ "float", 'f', "desc", OptionType_Float, &f },
		{ NULL, '\0', NULL, OptionType_Term, NULL }
	};
	LONGS_EQUAL(4, argc);
	CHECK(Option_Parse(&argc, (char***)&argv, op));
	DOUBLES_EQUAL(100.5, f, __FLT_EPSILON__);
	LONGS_EQUAL(2, argc);

	argv = argv2;
	argc = (sizeof(argv2)/sizeof(char*));
	CHECK_FALSE(Option_Parse(&argc, (char***)&argv, op));
	LONGS_EQUAL(3, argc);
}

TEST(Option, FailCase)
{
	OptionStruct op[] = {
		{ "int", 'i', "desc", OptionType_Int, NULL },
		{ NULL, '\0', NULL, OptionType_Term, NULL }
	};
	const char* argv1[] = {
		"dummy",
		"-f",
		"100.5",
	};
	const char* argv2[] = {
		"dummy",
		"-i"
	};

	const char** argv = argv1;
	int argc = (sizeof(argv1)/sizeof(char*));
	CHECK_FALSE(Option_Parse(&argc, (char***)&argv, op));
	LONGS_EQUAL(3, argc);

	argv = argv2;
	argc = (sizeof(argv2)/sizeof(char*));
	CHECK_FALSE(Option_Parse(&argc, (char***)&argv, op));
	LONGS_EQUAL(2, argc);
}

