/**
 * @file CollectInfo.c
 */
#include <bolib.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <bolib/file/RomFile.h>
#include "common/strres.h"
#include "common/Observer.h"
#include "smw/libsmw.h"
#include "mewthree/Signature.h"
#include "mewthree/InsInfo.h"
#include "mewthree/CollectInfo.h"

#define MatchFirstChunk(p, chunk) \
	if(0==memcmp(p, chunk, strlen(chunk))) \
	
#define MatchNextChunk(p, chunk) \
	else if(0==memcmp(p, chunk, strlen(chunk))) \

#define UnmatchChunk() \
	else \

/**
 * @brief It's verified whether it's Giepy's main RATS data.
 *        *** This function is called from RatsSearch function.
 *
 * @param data RATS data start pointer
 * @param len RATS data length
 *
 * @return true: GIEPY Main data / false: Others
 */
static bool IsGiepyMain(const uint8* data, const uint32 len)
{
	/* There is no data smaller than this. */
	if(0x600 > len)
	{
		return false;
	}

	if(0 != memcmp(Signature, data, SigLen))
	{
		return false;
	}
	data += SigLen;
	if(0 != memcmp("MAIN\x00\x00", data, 6))
	{
		return false;
	}

	return true;
}

/**
 * @brief Collect GIEPY installation info
 *
 * @param rom ROM file object
 * @param inf Install info
 * @param obs Observer
 *
 * @return true when success collect info, false when failer.
 */
bool CollectInfo(RomFile* rom, MewInsInfo* inf, Observers* obs)
{
	uint8* ptr;
	uint16 ratsSize;
	uint16 szCnt = 0;

	memset(inf, 0, sizeof(MewInsInfo));

	/* Check smw */
	if(false == IsSmw(rom))
	{
		obs->err(0, GSID_ROM_IS_NOT_SMW);
		return false;
	}

	/* get LM info */
	{
		const char LMSig[] = "Lunar Magic Version ";
		const size_t LMSigLen = strlen(LMSig);
		char sver[8];

		ptr = rom->GetSnesPtr(rom, 0x0ff0a0);
		if(NULL == ptr)
		{
			obs->err(0, GSID_COLLECT_LM_NOT_INSTALLED);
			return false;
		}

		if(0 != strncmp(LMSig, (char*)ptr, LMSigLen))
		{
			obs->err(0, GSID_COLLECT_LM_NOT_INSTALLED);
			return false;
		}
		strncpy_s(sver, 8, (char*)ptr+LMSigLen, 4);
		inf->lmVersion = atof(sver);
	}

	/* get ROM info */
	inf->lmExtBytesEnabled = false;
	inf->isSA1 = false;
	if(MapMode_SA1 == rom->mapmode_get(rom)) inf->isSA1 = true;
	if(*(rom->GetSnesPtr(rom, 0x0ef30f)) == 0x42) inf->lmExtBytesEnabled = true;

	/* search main data */
	inf->giepyMainAdr = rom->RatsSearch(rom, 0x108000, IsGiepyMain);
	if(ROMADDRESS_NULL == inf->giepyMainAdr)
	{
		inf->giepyIsInstalled = false;
		return true;
	}

	inf->giepyIsInstalled = true;

	/* read chunks */
	ptr = rom->GetSnesPtr(rom, inf->giepyMainAdr);
	ratsSize = read16(&ptr[4]);
	ptr += 8;	/* rats tag */
	ptr += 12;	/* Sig + MAIN + nulnul */

	inf->giepyCodePtr = NULL;
	/*printf("read start: $%06x + 12\n", inf->giepyMainAdr);*/
	while((0!=memcmp(&ptr[0], "TERM", 4)) && szCnt < ratsSize)
	{
		uint16 chunkSize;
		chunkSize = (uint16)(read16(&ptr[4]) + 6);

		/*printf("chunk: %c%c%c%c\n", ptr[0], ptr[1], ptr[2], ptr[3]);*/
		MatchFirstChunk(&ptr[0], "VER ")
		{
			uint16 insflg;

			inf->giepyIsSA1Code = false;
			inf->giepyIsExtBytesCode = false;
			inf->giepyIsPixiCompatible = false;

			insflg = read16(&ptr[6]);
			if(0 != (IFLG_SA1 & insflg)) inf->giepyIsSA1Code = true;
			if(0 != (IFLG_EXTRA_BYTES & insflg)) inf->giepyIsExtBytesCode = true;
			if(0 != (IFLG_PIXI_COMPATIBLE & insflg)) inf->giepyIsPixiCompatible = true;

			inf->giepyCodeVer = (uint16)(ptr[8] + ((uint16)ptr[9] << 8));
		}
		MatchNextChunk(&ptr[0], "CODE")
		{
			/* set main code ptr */
			inf->giepyCodePtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "EBLT")
		{
			/* set extra bytes adr */
			inf->giepyExtBytesPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "SPTT")
		{
			/* set sprite type adr */
			inf->giepySpTypeTableAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "STWP")
		{
			/* set normal sprite tweaks adr */
			inf->giepySpTweaksPtrAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "SPIP")
		{
			/* set normal sprite init adr */
			inf->giepySpInitPtrAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "SPMP")
		{
			/* set normal sprite main adr */
			inf->giepySpMainPtrAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "SHTP")
		{
			/* set shooter main adr */
			inf->giepyShootPtrAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "GENP")
		{
			/* set generator main adr */
			inf->giepyGenPtrAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "INIP")
		{
			/* set initializer init adr */
			inf->giepyInitPtrAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "SCI1")
		{
			/* set Layer1 scroller init adr */
			inf->giepyL1ScrInitAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "SCM1")
		{
			/* set Layer1 scroller main adr */
			inf->giepyL1ScrMainAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "SCI2")
		{
			/* set Layer1 scroller init adr */
			inf->giepyL2ScrInitAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "SCM2")
		{
			/* set Layer1 scroller main adr */
			inf->giepyL2ScrMainAdrPtr = &ptr[6];
		}
		MatchNextChunk(&ptr[0], "LOC ")
		{
			int i=6;
			/* get addresses */
			inf->giepyClsRelocAdrPtr = &ptr[i];
			inf->giepyExtRelocAdrPtr = &ptr[i+=3];
			inf->giepyExecutePtrLongPbAdrPtr = &ptr[i+=3];
			inf->giepySomethingNopAdrPtr = &ptr[i+=3];
			inf->giepySpriteNopAdrPtr = &ptr[i+=3];
			inf->giepyScrollL1NopPtr = &ptr[i+=3];
			inf->giepyScrollL2NopPtr = &ptr[i+=3];
			inf->giepyBackup02a9caPtr = &ptr[i+=3];
			inf->giepyTableGrps[0] = &ptr[i+=3];
			inf->giepyTableGrps[1] = &ptr[i+=3];
			inf->giepyTableGrps[2] = &ptr[i+=3];
			inf->giepyTableGrps[3] = &ptr[i+=3];
			inf->giepyTableGrps[4] = &ptr[i+=3];
		}
		UnmatchChunk()
		{
			obs->err(0, GSID_COLLECT_UNKNOWN_CHUNK, ptr[0], ptr[1], ptr[2], ptr[3]);
			return false;
		}

		/* increase pointer */
		szCnt = (uint16)(szCnt + chunkSize);
		ptr = ptr + chunkSize;
	}

	/* Check require data */
	if(NULL == inf->giepyCodePtr) return false;
	if(0 == inf->giepyCodeVer) return false;

	/* Consistency check */
	if((1.80 > inf->lmVersion) && (true == inf->giepyIsExtBytesCode))
	{
		/* Is lm version downgraded? this case is judged as a failure. */
		obs->err(0, GSID_COLLECT_LM_DOWNGRADED);
		return false;
	}

	/* collect sprite data length */
	if(true == inf->giepyIsExtBytesCode)
	{
		uint8* p;
		uint32 sa;
		p = rom->GetSnesPtr(rom, 0x0ef30c);
		sa = read24(p);
		p = rom->GetSnesPtr(rom, sa);
		if(NULL == p)
		{
			obs->err(0, GSID_COLLECT_BROKEN_ROM);
			return false;
		}
		/* restore previous install data (or initialize 3) */
		memcpy(inf->lmSpriteLengthBackup, p, 256*4);
	}

	/* check ok. */
	return true;
}
