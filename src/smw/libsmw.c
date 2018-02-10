/**
 * @file libsmw.c
 */
#include "common/types.h"
#include "common/puts.h"
#include "common/ReadWrite.h"
#include <assert.h>
#include "file/File.h"
#include "file/RomFile.h"
#include "smw/libsmw.h"

bool IsSmw(RomFile* rom)
{
	uint8* ptr;

	assert(rom);
	if(0x80000 > rom->size_get(rom)) return false;

	/* check rom */
	ptr = rom->GetSnesPtr(rom, 0x806f); /* Main loop */
	if(NULL == ptr)
	{
		return false;
	}
	if(0 == memcmp(ptr, (const void*)"\x58\xe6\x13\x20", 4))	/* CLI : INC $13 : JSR $**** */
	{
		uint32 x = read16(ptr+4);	/* It'll get $9322 : GetGamemode */
		ptr = rom->GetSnesPtr(rom, x);
		if(ptr != NULL)
		{
			if(0 == memcmp(ptr+7, (const void*)"\x91\x93\x0f\x94\x6f\x9f\xae\x96", 8))	/* Check Gamemode routine */
			{
				return true;
			}
		}
	}

	return false;
}

