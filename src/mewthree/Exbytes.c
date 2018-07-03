/**
 * @file Exbytes.c
 */
#include <bolib.h>
#include <bolib/file/RomFile.h>
#include "common/strres.h"
#include "common/Observer.h"
#include "mewthree/Rats.h"
#include "mewthree/Exbytes.h"

#define SPRDATA_MAX	15*255+1	/** 255 sprites with 12 extra bytes each */
#define SprAddrPtr	0x02ec00	/** snes:$05:ec00, 2bytes * 512 */
#define SprBankPtr	0x077100	/** snes:$0e:f100, 1byte  * 512 */

static void SetLevelAdr(RomFile* rom, uint level, uint32 addr)
{
	uint8* p;

	p = rom->GetPcPtr(rom, SprAddrPtr);
	write16(&p[(level<<1)], (addr&0xffff));

	p = rom->GetPcPtr(rom, SprBankPtr);
	p[level] = (uint8)(addr>>16);
}

static uint32 GetLevelAdr(RomFile* rom, uint level)
{
	uint32 bnk;
	uint32 adr;
	uint8* p;

	p = rom->GetPcPtr(rom, SprAddrPtr);
	adr = read16(&p[(level<<1)]);

	p = rom->GetPcPtr(rom, SprBankPtr);
	bnk = p[level];

	return (adr + (bnk<<16));
}

/**
 * @brief Rebuild the LEVEL data according to the length of EXTRA BYTES.
 *        This function refers to the following projects:
 *        - MeiMei
 *          https://github.com/Amaterasite/MeiMei
 *
 * @param rom ROM file object
 * @param before Data before change
 * @param after Data after change
 * @param obs Observer
 *
 * @return succeeded(true) / failure
 */
bool NormalizeSprDataLen(RomFile* rom, const uint8* before, const uint8* after, Observers* obs)
{
	uint i;
	uint j;
	uint level;
	bool isModified[256*4] = {false};
	bool needToUpdate = false;
	bool termed;
	uint32 sa;
	uint32 levelAdr;
	uint8* p;
	uint len;
	uint8 sprdata[SPRDATA_MAX];
	uint sprNum, sprGrp;
	uint sprInx;
	uint skips;

	/* check exbytes length update */
	for(i=0; i<256*4; i++)
	{
		if(before[i] != after[i])
		{
			/* update is detected */
			needToUpdate = true;
			isModified[i] = true;
		}
	}

	if(false == needToUpdate)
	{
		/* nothing to do, return */
		return true;
	}

	/*--- Update all level data ---*/
	for(level=0; level<0x200; level++)
	{
		needToUpdate = false;
		len = 0;

		levelAdr = GetLevelAdr(rom, level);
		/*printf("level %03X : $%06x\n", level, levelAdr);*/
		p = rom->GetSnesPtr(rom, levelAdr);
		if(NULL == p)
		{
			/* invalid addr... rom is broken */
			obs->err(0, GSID_EXBYTES_INVALID_ADDR, level, levelAdr);
			return false;
		}

		sprdata[len++] = p[0];
		i=1;
		termed = false;
		for(;i<SPRDATA_MAX-3;)
		{
			/* termination of sprite */
			if(0xff == p[i])
			{
				termed = true;
				sprdata[len++] = p[i];
				break;
			}

			sprGrp = (uint)((p[i+0] & 0x0c) << 6);
			sprdata[len++] = p[i+0];	/* YYYYEEsy */
			sprdata[len++] = p[i+1];	/* XXXXSSSS */
			sprNum = (uint)p[i+2];
			sprdata[len++] = p[i+2];	/* NNNNNNNN */
			/*if(0x105 == level) printf("sprite: %02x:%x\n", sprNum, sprGrp>>8);*/

			sprInx = sprGrp+sprNum;
			if(true == isModified[sprInx]) needToUpdate = true;

			skips = before[sprInx];
			if(after[sprInx] < skips) skips = after[sprInx];

			for(j=3; j<skips; j++)
			{
				sprdata[len++] = p[i+j];
			}
			for(; j<after[sprInx]; j++)
			{
				sprdata[len++] = 0;
			}
			i += before[sprInx];
		}
		if(false == termed)
		{
			/* sprite data is too large... (probably broken data) */
			obs->err(0, GSID_EXBYTES_SPRTERM_NOT_FOUND, level);
			return false;
		}

		if(true == needToUpdate)
		{
			/* delete previous data */
			if(0x100000 < (levelAdr&0x7fffff))
			{
				uint cleansize;
				cleansize = rom->RatsClean(rom, levelAdr-8);
				obs->debug(0, GSID_UNINSTALL_CLEANING, levelAdr-8, cleansize);
			}

			/* search free space for insert data */
			sa = rom->SearchFreeSpace(rom, len);
			if(ROMADDRESS_NULL == sa)
			{
				obs->err(0, GSID_ASSEMBLE_FREE_NOT_FOUND);
				return false;
			}

			/* insert rebuilt data */
			p = rom->GetSnesPtr(rom, sa);
			WriteRatsData(p, sprdata, len);
			SetLevelAdr(rom, level, sa+8);
			obs->debug(0, GSID_EXBYTES_SPR_DATA_REMAP, level, levelAdr, sa+8);
		}
	}

	/* succeeded. */
	return true;
}
