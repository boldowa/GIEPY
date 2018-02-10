extern "C"
{
#include "common/types.h"
#include "file/File.h"
#include "file/RomFile.h"
#include "../file/TestRomFile.h"
#include "smw/libsmw.h"
#include "common/puts.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(libsmw)
{
	RomFile* rom;
	RomFile* badrom;
	void setup()
	{
		rom = new_RomFile_ForTest("l1024");
		badrom = new_RomFile_ForTest("l256");
	}

	void teardown()
	{
		delete_RomFile(&rom);
		delete_RomFile(&badrom);
	}
};

TEST(libsmw, IsSmw)
{
	/* NULL pointer check */
	CHECK_FALSE(IsSmw(rom));

	/* ROM Open */
	LONGS_EQUAL(FileOpen_NoError, rom->Open(rom));

	/* Not SMW ROM check */
	CHECK_FALSE(IsSmw(rom));

	LONGS_EQUAL(RomType_LoRom, rom->type_get(rom));

	/* SMW check (1) */
	memcpy(rom->GetSnesPtr(rom, 0x806f),  (const void*)"\x58\xe6\x13\x20\x00\x00", 6);
	CHECK_FALSE(IsSmw(rom));

	/* SMW check (2) */
	memcpy(rom->GetSnesPtr(rom, 0x8073),  (const void*)"\x22\x93", 2);
	CHECK_FALSE(IsSmw(rom));

	/* SMW check (3) */
	memcpy(rom->GetSnesPtr(rom, 0x9329),  (const void*)"\x91\x93\x0f\x94\x6f\x9f\xae\x96", 8);
	CHECK(IsSmw(rom));
}

TEST(libsmw, IsLMInstalled)
{
	const char LMSig[] = "Lunar Magic Version ";
	const size_t LMSigLen = strlen(LMSig);
	uint8 *p;

	LONGS_EQUAL(FileOpen_NoError, rom->Open(rom));
	LONGS_EQUAL(FileOpen_NoError, rom->Open(badrom));

	/* NULL ADDRESS check */
	CHECK_FALSE(IsLMInstalled(badrom));

	/* LMSig notfound check */
	CHECK_FALSE(IsLMInstalled(rom));

	/* insert LM sig */
	p = rom->GetSnesPtr(rom, 0x0ff0a0);
	memcpy(p, LMSig, LMSigLen);

	/* LM version unmatch test */
	CHECK_FALSE(IsLMInstalled(rom));

	/* LM version */
	memcpy(p+LMSigLen, "1.80", 4);
	CHECK_FALSE(IsLMInstalled(rom));

	/* min LM version */
	memcpy(p+LMSigLen, "1.81", 4);
	CHECK(IsLMInstalled(rom));

	/* max ver */
	memcpy(p+LMSigLen, "9.99", 4);
	CHECK(IsLMInstalled(rom));
}
