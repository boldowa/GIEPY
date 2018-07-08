/**
 * @file Uninstall.c
 *   ... GIEPY code uninstaller
 */
#include <bolib.h>
#include <assert.h>
#include <memory.h>
#include <setjmp.h>
#include <bolib/file/RomFile.h>
#include "mewthree/Signature.h"
#include "common/strres.h"
#include "common/Observer.h"
#include "mewthree/InsInfo.h"
#include "mewthree/Exbytes.h"
#include "mewthree/Uninstall.h"

/**
 * @brief The data for restore rom image.
 */
typedef struct _RestoreData {
	uint32	address;	/* snes address */
	uint32	length;
	uint8*	data;
}RestoreData;


/**
 * @brief Check data
 *
 * @param data data ptr
 * @param len data length
 *
 * @return 
 */
static bool IsGiepySprData(const uint8* data, const uint32 len)
{
	/* There is no data smaller than this. */
	if((SpSigLen+1) > len) /* minimun: signature + rtl */
	{
		return false;
	}

	if(0 != memcmp(SpSignature, data, SpSigLen))
	{
		return false;
	}

	return true;
}

/**
 * @brief Check data
 *
 * @param data data ptr
 * @param len data length
 *
 * @return 
 */
static bool IsGiepyLibData(const uint8* data, const uint32 len)
{
	/* There is no data smaller than this. */
	if((SigLen+6) > len)
	{
		return false;
	}

	if(0 != memcmp(Signature, data, SigLen))
	{
		return false;
	}
	data += SigLen;
	if(0 != memcmp("LIB_\x00\x00", data, 6))
	{
		return false;
	}

	return true;
}


/**
 * @brief Restore sub function
 *
 * @param r restore data
 * @param rom ROM file object
 * @param obs Observer
 *
 * @return success(true) or failure(false)
 */
static bool RestoreSub(RestoreData* r, RomFile* rom, Observers* obs)
{
	uint8* ptr;
	RestoreData* restore;

	for(restore=r; NULL != restore->data; restore++)
	{
		ptr = rom->GetSnesPtr(rom, restore->address);
		if(NULL == ptr)
		{
			/* Unknown rom error */
			obs->err(0, GSID_UNINSTALL_RESADR_NULL);
			return false;
		}
		if((ptr + restore->length) > rom->GetPcPtr(rom, (uint32)rom->size_get(rom)-1))
		{
			/* Unknown rom error */
			obs->err(0, GSID_UNINSTALL_RESADR_NULL);
			return false;
		}

		obs->debug(0, GSID_UNINSTALL_TWEAKING, restore->address, restore->length);
		memcpy(ptr, restore->data, restore->length);
	}

	return true;
}

/**
 * @brief Restore ROM image for code version 0x0100 - 0x****
 *
 * @param rom ROM file oject
 * @param obs observer
 *
 * @return success(true) or failure(false)
 */
static bool Restore_v0100(RomFile* rom, Observers* obs)
{
	/************************************************/
	/* Restore data (common)			*/
	/************************************************/
	static RestoreData CommonRestore[] = {
		/* load.asm */
		{ 0x02a837, 4, (uint8*)"\x29\x10\x85\x02"     },
		{ 0x02a861, 4, (uint8*)"\xb7\xce\x85\x05"     },
		{ 0x02a846, 4, (uint8*)"\xc8\xc8\xe8\x80"     },

		/* normspr.asm */
		{ 0x02a971, 4, (uint8*)"\xc8\xc8\xa5\x04"     },
		{ 0x02a8ee, 1, (uint8*)"\x06"                 },
		{ 0x02a8fd, 1, (uint8*)"\x06"                 },
		{ 0x02a915, 1, (uint8*)"\x06"                 },
		{ 0x02a91f, 1, (uint8*)"\x06"                 },
		{ 0x02a933, 1, (uint8*)"\x06"                 },
		{ 0x02a964, 1, (uint8*)"\x0d"                 },
		{ 0x02a94c, 1, (uint8*)"\x0d"                 },
		{ 0x0187a7, 4, (uint8*)"\x22\x59\xda\x02"     },
		{ 0x0182b3, 2, (uint8*)"\xa7\x87"             },
		{ 0x0187c5, 4, (uint8*)"\x22\xaf\xdc\x02"     },

		/* shooter.asm */
		{ 0x02b39e, 4, (uint8*)"\x90\x03\xde\xab"     },

		/* generator.asm */
		{ 0x02a8b4, 4, (uint8*)"\x38\xe9\xcb\x1a"     },
		{ 0x02b003, 4, (uint8*)"\xa4\x9d\xd0\x23"     },

		/* initializer.asm : nothing */

		/* scroll.asm */
		{ 0x02a874, 5, (uint8*)"\xa5\x05\x38\xe9\xe7" },
		{ 0x05bcd6, 4, (uint8*)"\x8b\x4b\xab\x9c"     },

		/* extended.asm */
		{ 0x029b1b, 4, (uint8*)"\xa4\x9d\xd0\x08"     },

		/* cluster.asm : nothing */

		/* owspr.asm : nothing */

		/* tweaks.asm */
		{ 0x00a1ad, 5, (uint8*)"\x74\x1a\xca\x10\xfb" },

		/* end */
		{ 0x000000, 0,          NULL         }
	};


	/************************************************/
	/* Restore data (LoROM)				*/
	/************************************************/
	static RestoreData LoRomRestore[] = {
		/* normspr.asm */
		{ 0x02a9a3, 4, (uint8*)"\xda\xb5\x9e\xaa"     },
		{ 0x02a9d4, 4, (uint8*)"\x9d\xe2\x1f\xc8"     },
		{ 0x018172, 5, (uint8*)"\xa9\x08\x9d\xc8\x14" },
		{ 0x0185c3, 5, (uint8*)"\x9c\x91\x14\xb5\x9e" },
		{ 0x07f787, 4, (uint8*)"\x9d\xa0\x15\x6b"     },
		{ 0x018127, 5, (uint8*)"\xbd\xc8\x14\xf0\x25" },
		{ 0x01d43e, 4, (uint8*)"\x9e\xc8\x14\x60"     },
		{ 0x0180b5, 5, (uint8*)"\xad\xb8\x18\xf0\x04" },

		/* shooter.asm */
		{ 0x02aba3, 5, (uint8*)"\xe9\xc8\x9d\x83\x17" },
		{ 0x02abeb, 4, (uint8*)"\xc8\xa6\x02\xe8"     },

		/* scroll.asm */
		{ 0x05bc79, 4, (uint8*)"\xad\x9d\x00\xd0"     },
		{ 0x05bcaa, 4, (uint8*)"\xad\x3f\x14\xf0"     },

		/* cluster.asm */
		{ 0x02f815, 4, (uint8*)"\xbd\x92\x18\xf0"     },

		/* tweaks.asm */
		{ 0x01c089, 4, (uint8*)"\xbd\xd4\x14\x9d"     },
		{ 0x02ac53, 5, (uint8*)"\xe2\x10\x9c\x3e\x14" },

		{ 0x000000, 0,          NULL         }
	};

	static RestoreData FastRomRestore[] = {
		/* normspr.asm */
		{ 0x0187a7, 4, (uint8*)"\x22\x59\xda\x82"     },
		{ 0x0187c5, 4, (uint8*)"\x22\xaf\xdc\x82"     },

		{ 0x000000, 0,          NULL         }
	};


	/************************************************/
	/* Restore data (SA-1 ROM)			*/
	/************************************************/
	static RestoreData SA1Restore[] = {
		/* normspr.asm */
		{ 0x02a9a3, 4, (uint8*)"\xda\xb2\xb4\xaa"     },
		{ 0x02a9d4, 4, (uint8*)"\x9d\xd6\x7f\xc8"     },
		{ 0x018172, 5, (uint8*)"\xa9\x08\x9d\x42\x32" },
		{ 0x0185c3, 5, (uint8*)"\x9c\x91\x74\xb2\xb4" },
		{ 0x07f787, 4, (uint8*)"\x9d\x76\x33\x6b"     },
		{ 0x018127, 5, (uint8*)"\xbd\x42\x32\xf0\x25" },
		{ 0x01d43e, 4, (uint8*)"\x9e\x42\x32\x60"     },
		{ 0x0180b5, 5, (uint8*)"\xad\xb8\x78\xf0\x04" },

		/* shooter.asm */
		{ 0x02aba3, 5, (uint8*)"\xe9\xc8\x9d\x83\x77" },
		{ 0x02abeb, 4, (uint8*)"\xc8\xa6\x02\x5c"     },

		/* scroll.asm */
		{ 0x05bc79, 4, (uint8*)"\xad\x9d\x30\xd0"     },
		{ 0x05bcaa, 4, (uint8*)"\xad\x3f\x74\xf0"     },

		/* cluster.asm */
		{ 0x02f815, 4, (uint8*)"\xbd\x92\x78\xf0"     },

		/* tweaks.asm */
		{ 0x01c089, 4, (uint8*)"\xbd\x58\x32\x9d"     },
		{ 0x02ac53, 5, (uint8*)"\xe2\x10\x9c\x3e\x74" },

		{ 0x000000, 0,          NULL         }
	};

	/* restore common data */
	if(false == RestoreSub(CommonRestore, rom, obs)) return false;

	if(MapMode_20H == rom->mapmode_get(rom))
	{
		if(false == RestoreSub(FastRomRestore, rom, obs)) return false;
	}

	/* restore lo / sa1 ROM data */
	if(MapMode_SA1 == rom->mapmode_get(rom)) return RestoreSub(SA1Restore, rom, obs);
	return RestoreSub(LoRomRestore, rom, obs);
}

/**
 * @brief Tweaks information uninstaller
 *
 * @param rom ROM file object
 * @param inf system information
 * @param obs observer
 * @param grp tweaks pointer
 *
 * @return succeeded / failure
 */
static bool UninstallSpritesTweaks(RomFile* rom, MewInsInfo* inf, Observers* obs, uint8* grp)
{
	uint8* p;
	uint16 sz, szc;
	uint32 grpsa;
	uint32 cleansize;

	/* get rom data pointer */
	grpsa = read24(grp);
	if(0 == grpsa)
	{
		/* data isn't exists */
		return true;
	}
	p = rom->GetSnesPtr(rom, grpsa);
	if(NULL == p)
	{
		obs->err(0, GSID_UNINSTALL_DATA_BROKEN);
		return false;
	}

	/* Verily RATS tag */
	if(0 != memcmp(p, "STAR", 4))
	{
		obs->err(0, GSID_UNINSTALL_INVALID_RATS);
		return false;
	}
	sz = read16(p+4);
	szc = read16(p+6);
	if(sz != (szc^0xffff))
	{
		obs->err(0, GSID_UNINSTALL_INVALID_RATS);
		return false;
	}

	/* Verify table */
	sz++;
	if(0 != sz%16)
	{
		obs->err(0, GSID_UNINSTALL_INVALID_RATS);
		return false;
	}

	cleansize = rom->RatsClean(rom, grpsa);
	if(0 == cleansize)
	{
		obs->err(0, GSID_UNINSTALL_RATSCLEAN_NG);
		return false;
	}
	obs->debug(0, GSID_UNINSTALL_CLEANING, grpsa, cleansize);

	write24(grp, 0);
	return true;
}

/**
 * @brief The uninstaller for "Sprite" section sprite
 *
 * @param rom ROM file object
 * @param inf system information
 * @param obs observer
 * @param grp sprite group info pointer
 *
 * @return succeeded / failure
 */
static bool UninstallSpritesSub1(RomFile* rom, MewInsInfo* inf, Observers* obs, uint8* grp)
{
	uint8* p;
	uint16 sz, szc;
	uint32 grpsa;
	uint32 cleansize;

	/* get rom data pointer */
	grpsa = read24(grp);
	if(0 == grpsa)
	{
		/* data isn't exists */
		return true;
	}
	p = rom->GetSnesPtr(rom, grpsa);
	if(NULL == p)
	{
		obs->err(0, GSID_UNINSTALL_DATA_BROKEN);
		return false;
	}

	/* Verily RATS tag */
	if(0 != memcmp(p, "STAR", 4))
	{
		obs->err(0, GSID_UNINSTALL_INVALID_RATS);
		return false;
	}
	sz = read16(p+4);
	szc = read16(p+6);
	if(sz != (szc^0xffff))
	{
		obs->err(0, GSID_UNINSTALL_INVALID_RATS);
		return false;
	}

	/* Verify table */
	sz++;
	if(0 != sz%4)
	{
		obs->err(0, GSID_UNINSTALL_INVALID_RATS);
		return false;
	}

	/* clean table */
	cleansize = rom->RatsClean(rom, grpsa);
	if(0 == cleansize)
	{
		obs->err(0, GSID_UNINSTALL_RATSCLEAN_NG);
		return false;
	}
	obs->debug(0, GSID_UNINSTALL_CLEANING, grpsa, cleansize);

	write24(grp, 0);
	return true;
}

/**
 * @brief The uninstaller for extended and cluster sprite info
 *
 * @param rom ROM file object
 * @param inf system information
 * @param obs observer
 * @param grp extended or cluster reloc pointer
 * @param nopadr NOP routine pointer
 *
 * @return succeeded / failure
 */
static bool UninstallSpritesSub2(RomFile* rom, MewInsInfo* inf, Observers* obs, uint8* grp, const uint32 nopadr)
{
	uint8* p;
	uint8* ptr;
	uint16 sz, szc;
	uint32 grpsa;
	uint32 sa;
	uint32 cleansize;

	/* get routine reloc pointer */
	sa = read24(grp);
	ptr = rom->GetSnesPtr(rom, sa);
	if(NULL == ptr)
	{
		obs->err(0, GSID_UNINSTALL_DATA_BROKEN);
		return false;
	}
	/* get rom data pointer */
	grpsa = read24(ptr);
	if(nopadr == grpsa) /* empty check */
	{
		/* data isn't installed. nothing to do. */
		return true;
	}
	write24(ptr, nopadr);	/* jsl routine => rtl only */
	p = rom->GetSnesPtr(rom, grpsa-8);
	if(NULL == p)
	{
		obs->err(0, GSID_UNINSTALL_DATA_BROKEN);
		return false;
	}

	/* Verily RATS tag */
	if(0 != memcmp(p, "STAR", 4))
	{
		obs->err(0, GSID_UNINSTALL_INVALID_RATS);
		return false;
	}
	sz = read16(p+4);
	szc = read16(p+6);
	if(sz != (szc^0xffff))
	{
		obs->err(0, GSID_UNINSTALL_INVALID_RATS);
		return false;
	}

	/* Verify table */
	sz = (uint16)(sz - 3);	/* jsl ExecutePtrLong */
	if(0 != sz%3)
	{
		obs->err(0, GSID_UNINSTALL_INVALID_RATS);
		return false;
	}

	/* clean table */
	cleansize = rom->RatsClean(rom, grpsa-8);
	if(0 == cleansize)
	{
		obs->err(0, GSID_UNINSTALL_RATSCLEAN_NG);
		return false;
	}
	obs->debug(0, GSID_UNINSTALL_CLEANING, grpsa-8, cleansize);

	return true;
}


#ifdef CPPUTEST
void uniFree(void* data){free(data);}
#else
#  define uniFree free
#endif
/**
 * @brief Uninstall all sprites from ROM image.
 *
 * @param rom ROM file object
 * @param inf Install info
 * @param obs observer
 *
 * @return 
 */
static bool UninstallSprites(RomFile* rom, MewInsInfo* inf, Observers* obs)
{
	jmp_buf e;
	uint32 sa = 0x108000;
	int i;
	uint32 cleansize;

	assert(rom);
	assert(inf);
	assert(obs);

	obs->debug(0, GSID_UNINSTALL_SPRITES);
	/* Uninstall all codes */
	while(sa != ROMADDRESS_NULL)
	{
		sa = rom->RatsSearch(rom, sa, IsGiepySprData);
		if(ROMADDRESS_NULL != sa)
		{
			cleansize =  rom->RatsClean(rom, sa);
			if(0 == cleansize)
			{
				obs->err(0, GSID_UNINSTALL_RATSCLEAN_NG);
				return false;
			}
			obs->debug(0, GSID_UNINSTALL_CLEANING, sa, cleansize);
		}
	}

	/* init all sprite type */
	for(i=0; i<0x400; i++) inf->giepySpTypeTableAdrPtr[i] = 0;

	/* clean tables */
	if(0 == setjmp(e)) /* try */
	{
		/* uninstall sprites */
		obs->debug(0, GSID_UNINSTALL_PROCESS_GRP, 1);
		if(false == UninstallSpritesSub1(rom, inf, obs, inf->giepyTableGrps[0])) longjmp(e, 1);
		obs->debug(0, GSID_UNINSTALL_PROCESS_GRP, 2);
		if(false == UninstallSpritesSub1(rom, inf, obs, inf->giepyTableGrps[1])) longjmp(e, 1);
		obs->debug(0, GSID_UNINSTALL_PROCESS_GRP, 3);
		if(false == UninstallSpritesSub1(rom, inf, obs, inf->giepyTableGrps[2])) longjmp(e, 1);
		obs->debug(0, GSID_UNINSTALL_PROCESS_GRP, 4);
		if(false == UninstallSpritesSub1(rom, inf, obs, inf->giepyTableGrps[3])) longjmp(e, 1);

		/* uninstall cluster / extended */
		obs->debug(0, GSID_UNINSTALL_EXTENDED);
		if(false == UninstallSpritesSub2(rom, inf, obs, inf->giepyExtRelocAdrPtr, read24(inf->giepySpriteNopAdrPtr))) longjmp(e, 1);
		obs->debug(0, GSID_UNINSTALL_CLUSTER);
		if(false == UninstallSpritesSub2(rom, inf, obs, inf->giepyClsRelocAdrPtr, read24(inf->giepySpriteNopAdrPtr))) longjmp(e, 1);

		/* uninstall tweaks */
		obs->debug(0, GSID_UNINSTALL_TWEAKS);
		if(false == UninstallSpritesTweaks(rom, inf, obs, inf->giepyTableGrps[4])) longjmp(e, 1);
	}
	else /* catch */
	{
		return false;
	}
	return true;
}

/**
 * @brief Uninstall GIEPY's libraries data from ROM image.
 *
 * @param rom ROM file object
 * @param inf Install info
 * @param obs observer
 *
 * @return 
 */
static bool UninstallLibraries(RomFile* rom, MewInsInfo* inf, Observers* obs)
{
	uint32 sa = 0x108000;
	uint32 cleansize;

	assert(rom);
	assert(inf);
	assert(obs);

	obs->debug(0, GSID_UNINSTALL_LIBRARIES);
	while(sa != ROMADDRESS_NULL)
	{
		sa = rom->RatsSearch(rom, sa, IsGiepyLibData);
		if(ROMADDRESS_NULL != sa)
		{
			cleansize = rom->RatsClean(rom, sa);
			if(0 == cleansize)
			{
				obs->err(0, GSID_UNINSTALL_RATSCLEAN_NG);
				return false;
			}
			obs->debug(0, GSID_UNINSTALL_CLEANING, sa, cleansize);
		}
	}

	return true;
}


/*============================================================================*/
/*  Public								      */
/*============================================================================*/

/**
 * @brief Uninstall GIEPY Main data from ROM image
 *
 * @param rom ROM file object
 * @param inf Install info
 * @param obs observer
 *
 * @return 
 */
bool UninstallMainData(RomFile* rom, MewInsInfo* inf, Observers* obs)
{
	int i;
	uint8*	p;
	uint32 cleansize;

	assert(rom);
	assert(inf);
	assert(obs);

	obs->debug(0, GSID_UNINSTALL_COREDATA);
	/* check giepy data uninstalled */
	for(i=0; i<4; i++)
	{
		if(0 != read24(inf->giepyTableGrps[i]))
		{
			/* data isn't uninstalled(!!Coding ERROR!!) */
			obs->err(0, GSID_UNINSTALL_LEAKDATA);
			return false;
		}
	}
	/* omit extended / cluster check */


	/* nothing to do when giepy isn't installed. */
	if(false == inf->giepyIsInstalled)
	{
		return false;
	}

	/* Restore jsl address */
	p = rom->GetPcPtr(rom, 0x0129ca);		/* snes: $02:a9ca */
	memcpy(p, inf->giepyBackup02a9caPtr, 3);

	cleansize = rom->RatsClean(rom, inf->giepyMainAdr);
	if(0 == cleansize)
	{
		/* broken rats tag. */
		obs->err(0, GSID_UNINSTALL_RATSCLEAN_NG);
		return false;
	}
	obs->debug(0, GSID_UNINSTALL_CLEANING, inf->giepyMainAdr, cleansize);

	if(inf->giepyIsExtBytesCode)
	{
		memset(rom->GetSnesPtr(rom, 0x0ef30c), 0xff, 4);
	}


	obs->debug(0, GSID_UNINSTALL_RESTORE);
	switch(inf->giepyCodeVer)
	{
		case 0x0001:	/* v00.01 : (*** dev ver. ***)       */
		case 0x0100:	/* v01.00 : first version            */
		case 0x0101:	/* v01.01 : github #1,#5,#6          */
		case 0x0102:	/* v01.02 : github #7,#8,#10,#13     */
			return Restore_v0100(rom, obs);

		default:
			/* coding err: uninstall function is missing. */
			obs->err(0, GSID_UNINSTALL_NOTIMP_RES);
			break;
	}

	return false;
}

/**
 * @brief Uninstall GIEPY libraries and sprites.
 *
 * @param rom ROM file object
 * @param inf Install info
 * @param obs observer
 *
 * @return true:succeed / false:failure
 */
bool CleanData(RomFile* rom, MewInsInfo* inf, Observers* obs)
{
	if(inf->giepyIsInstalled)
	{
		if(false == UninstallLibraries(rom, inf, obs)) return false;
		if(false == UninstallSprites(rom, inf, obs))   return false;
	}

	return true;
}

/**
 * @brief Uninstall all data about GIEPY from ROM image
 *
 * @param rom ROM file object
 * @param inf Install info
 * @param obs observer
 *
 * @return 
 */
bool UninstallAllData(RomFile* rom, MewInsInfo* inf, Observers* obs)
{
	if(false == inf->giepyIsInstalled)
	{
		obs->err(0, GSID_UNINSTALL_NOT_INSTALLED);
		return false;
	}

	if(false == UninstallLibraries(rom, inf, obs)) return false;
	if(false == UninstallSprites(rom, inf, obs))   return false;
	if(false == UninstallMainData(rom, inf, obs))  return false;

	/* Fix sprite length */
	if(inf->giepyIsExtBytesCode)
	{
		uint8 res[256*4];
		memset(res, 3, 256*4);
		if(false == NormalizeSprDataLen(rom, (uint8*)inf->lmSpriteLengthBackup, res, obs)) return false;
	}

	return true;
}

