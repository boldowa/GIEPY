/**
 * @file Sprites.c
 *   ... construct and insert sprite tables...
 */
#include "common/types.h"
#include <assert.h>
#include "common/List.h"
#include "common/ReadWrite.h"
#include "common/strres.h"
#include "common/InsertList.h"
#include "common/Observer.h"
#include "common/CfgData.h"
#include "file/File.h"
#include "file/RomFile.h"
#include "mewthree/MewEnv.h"
#include "mewthree/InsInfo.h"
#include "mewthree/Exbytes.h"

#include "mewthree/Sprites.h"

/**
 * @brief Table optimization structure
 */
typedef struct _optinfo {
	int nums;
	int min;
	int max;
}opi;

/**
 * @brief Caculate table length
 *
 * @param o optimize info
 *
 * @return table data lenght
 */
static uint CalcTableLength(opi* o)
{
	uint len = 0;
	int i;

	for(i=0;i<4;i++)
	{
		if(0 != o[i].nums) len += (uint)(o[i].max - o[i].min + 1);
	}

	return len;
}

/**
 * @brief Add sprite tweaks data into ROM image
 *
 * @param dst ROM destination pointer
 * @param sa SNES Address
 * @param o optimization info
 * @param tbl src tweaks table
 * @param ptr ptr for save the table location 
 */
static void AddTweaks(uint8** dst, uint32* sa, opi* o, const uint8* tbl, uint8* ptr)
{
	int i, j;

	for(i=0;i<4;i++)
	{
		if(0 == o[i].nums)
		{
			(*ptr++)=0;
			write24(ptr, 0);
			ptr+=3;
			continue;
		}
		
		(*ptr++) = (uint8)o[i].min;
		write24(ptr, (*sa));
		ptr += 3;
		for(j=o[i].min;j<=o[i].max;j++)
		{
			memcpy((*dst), &tbl[((i<<8)+j)<<4], 16);
			(*dst) += 16;
			(*sa) += 16;
		}
	}
}

/**
 * @brief Add routine address into ROM image
 *
 * @param dst ROM dest pointer
 * @param sa dest SNES address
 * @param o optimization info
 * @param tbl src table pointer
 * @param ptr ptr for save the table location 
 */
static void AddData(uint8** dst, uint32* sa, opi* o, const uint8* tbl, uint8* ptr)
{
	int i, j;

	for(i=0;i<4;i++)
	{
		if(0 == o[i].nums)
		{
			(*ptr++)=0;
			write24(ptr, 0);
			ptr+=3;
			continue;
		}
		
		(*ptr++) = (uint8)o[i].min;
		write24(ptr, (*sa));
		ptr += 3;
		for(j=o[i].min;j<=o[i].max;j++)
		{
			memcpy((*dst), &tbl[((i<<8)+j)*4], 4);
			(*dst) += 4;
			(*sa) += 4;
		}
	}
}

/**
 * @brief Fill sprite tweaks table with miscellaneous data
 *
 * @param tbl tweaks table pointer
 */
static void InitTweaks(uint8* tbl)
{
	int i, j;
	static const uint8 initbl1[][6] = {		/* best couple */
		{0x48,0x69,0x6e,0x61,0x21,0x20},
		{0x4d,0x69,0x72,0x61,0x69,0x21}
	};
	static const uint8 initbl2[][10] = {
		{0x83,0x4a,0x83,0x8c,0x83,0x93,0x8e,0x51,0x8f,0xe3},	/* RANDOM CP932 MESSAGES */
		{0x82,0xcb,0x82,0xc1,0x82,0xd5,0x82,0xcb,0x82,0xd5},
		{0x82,0xac,0x82,0xa0,0x82,0xbf,0x82,0xe1,0x82,0xf1},
		{0x83,0x60,0x83,0x6d,0x82,0xbf,0x82,0xe1,0x82,0xf1},
		{0x83,0x60,0x83,0x6d,0x82,0xbf,0x82,0xe1,0x82,0xf1},
		{0x83,0x8a,0x83,0x5b,0x8b,0xb3,0x8a,0xaf,0x81,0x49},
		{0x83,0x6e,0x83,0x80,0x83,0x7d,0x83,0x93,0x8b,0xb3},
		{0x95,0x73,0x92,0x6d,0x89,0xce,0x82,0xc5,0x82,0xb7},
	};

	/* step 1 - best couple */
	for(i=0,j=10; i<256*4; i++,j+=16) memcpy(&tbl[j], initbl1[i&1], 6);

	/* step 2 - random power */
	for(i=0,j=0; i<256*4; i++,j+=16) memcpy(&tbl[j], initbl2[i&7], 10);
}

/**
 * @brief Fill subroutine pointer table with miscellaneous data
 *
 * @param tbl subroutine table pointer
 */
static void InitTable(uint8* tbl)
{
	int i, j;
	static const uint8 initbl[][4] = {	/* https://line.me/S/sticker/1700772 */
		{0x4e,0x65,0x70,0x74},
		{0x75,0x6e,0x65,0x21}
	};

	for(i=j=0; j<4*256; j++)
	{
		tbl[i] = initbl[j&1][i&3];
		i++;
		tbl[i] = initbl[j&1][i&3];
		i++;
		tbl[i] = initbl[j&1][i&3];
		i++;
		tbl[i] = initbl[j&1][i&3];
		i++;
	}
}

/**
 * @brief Update optimization information
 *
 * @param o optimizization info
 * @param grp sprite insert group
 * @param num sprite insert number
 */
static void OpiUpdate(opi* o, int grp, int num)
{
	if(0 == o[grp].nums)
	{
		o[grp].min = num;
		o[grp].max = num;
	}
	else
	{
		if(o[grp].min > num)
			o[grp].min = num;

		if(o[grp].max < num)
			o[grp].max = num;
	}
	o[grp].nums++;
}

/**
 * @brief Insert sprite information into ROM image.
 *
 * @param rom ROM file object
 * @param inf system information
 * @param iList Insert list
 * @param env Environment
 * @param obs Observer
 *
 * @return succeeded / failure
 */
bool InsertSprites(RomFile* rom, MewInsInfo* inf, List* iList, MewEnvStruct* env, Observers* obs)
{
	Iterator* it;
	Iterator* it_next;
	InsertItem* item;
	uint tbllen;
	uint32 sa;

	/* grp 1 */
	uint8 sprinit[4][4*256];
	uint8 sprmain[4][4*256];
	opi opi_spr[4] = {0};
	opi opi_tw[4] = {0};

	/* grp 2 */
	uint8 shootmain[4][4*256];
	uint8 genmain[4][4*256];
	uint8 initinit[4][4*256];
	opi opi_shoot[4] = {0};
	opi opi_gen[4] = {0};
	opi opi_ini[4] = {0};

	/* grp 3 */
	uint8 l1scrinit[4][4*256];
	uint8 l1scrmain[4][4*256];
	/* grp 4 */
	uint8 l2scrinit[4][4*256];
	uint8 l2scrmain[4][4*256];
	opi opi_scr[4] = {0};

	/* tweaks for normal & original sprites */
	uint8 tweaks[4][16*256];


	/* init table */
	InitTable((uint8*)sprinit);
	InitTable((uint8*)sprmain);
	InitTable((uint8*)shootmain);
	InitTable((uint8*)genmain);
	InitTable((uint8*)initinit);
	InitTable((uint8*)l1scrinit);
	InitTable((uint8*)l1scrmain);
	InitTable((uint8*)l2scrinit);
	InitTable((uint8*)l2scrmain);
	InitTweaks((uint8*)tweaks);

	/* data process */
	it = iList->begin(iList);
	while(NULL != it)
	{
		item = (InsertItem*)it->data(it);

		/* check sprite type */
		if(SpriteType_Sprite != item->type)
		{
			it = it->next(it);
			continue;
		}

		/* update sprite type */
		inf->giepySpTypeTableAdrPtr[(item->grp<<8)+item->number] = item->cfg->sprite_type;

		/* update extra bytes length */
		if(inf->giepyIsExtBytesCode)
		{
			inf->giepyExtBytesPtr[(item->grp<<8)+item->number] = (uint8)(item->cfg->extra_byte_nums+3);
		}

		switch(item->cfg->sprite_type)
		{
			case 1: /* custom sprite */
				OpiUpdate(opi_spr, item->grp, item->number);
				OpiUpdate(opi_tw, item->grp, item->number);
				write24(&sprinit[item->grp][item->number*4], item->cfg->insinf1.initloc);
				sprinit[item->grp][item->number*4+3] = item->cfg->unique_info;
				write24(&sprmain[item->grp][item->number*4], item->cfg->insinf1.mainloc);
				sprmain[item->grp][item->number*4+3] = item->cfg->unique_info;
				goto sprshare;

			case 0: /* orig sprite */
				OpiUpdate(opi_tw, item->grp, item->number);
				
			sprshare: /* custom sprite share */
				/* insert tweaks */
				tweaks[item->grp][(item->number<<4)+ 0] = item->cfg->new_code_flag;
				tweaks[item->grp][(item->number<<4)+ 1] = item->cfg->orig_sprite_number;
				memcpy(&tweaks[item->grp][(item->number<<4)+ 2], item->cfg->tweaks, 6);
				memcpy(&tweaks[item->grp][(item->number<<4)+ 8], item->cfg->extra_props, 2);
				break;

			case 2: /* shooter */
				OpiUpdate(opi_shoot, item->grp, item->number);
				write24(&shootmain[item->grp][item->number*4], item->cfg->insinf1.mainloc);
				shootmain[item->grp][item->number*4+3] = item->cfg->unique_info;
				break;

			case 3: /* generator */
				OpiUpdate(opi_gen, item->grp, item->number);
				write24(&genmain[item->grp][item->number*4], item->cfg->insinf1.mainloc);
				genmain[item->grp][item->number*4+3] = item->cfg->unique_info;
				break;

			case 4: /* initializer */
				OpiUpdate(opi_ini, item->grp, item->number);
				write24(&initinit[item->grp][item->number*4], item->cfg->insinf1.initloc);
				initinit[item->grp][item->number*4+3] = item->cfg->unique_info;
				break;

			case 5: /* scroller */
				OpiUpdate(opi_scr, item->grp, item->number);
				/* layer 1 */
				write24(&l1scrinit[item->grp][item->number*4], item->cfg->insinf1.initloc);
				l1scrinit[item->grp][item->number*4+3] = item->cfg->unique_info;
				write24(&l1scrmain[item->grp][item->number*4], item->cfg->insinf1.mainloc);
				l1scrmain[item->grp][item->number*4+3] = item->cfg->unique_info;
				/* layer 2 */
				write24(&l2scrinit[item->grp][item->number*4], item->cfg->insinf2.initloc);
				l2scrinit[item->grp][item->number*4+3] = item->cfg->unique_info;
				write24(&l2scrmain[item->grp][item->number*4], item->cfg->insinf2.mainloc);
				l2scrmain[item->grp][item->number*4+3] = item->cfg->unique_info;
				break;

			default:
				return false;
		}

		it_next = it->next(it);
		iList->remove(iList, it);
		it = it_next;
	}

	/*--- calculate tweks length ---*/
	tbllen = 0;
	tbllen += CalcTableLength(opi_tw);
	tbllen <<= 4;
	if(0 == tbllen)
	{
		write24(inf->giepyTableGrps[4], 0);
	}
	else
	{
		uint8* dst;

		sa = rom->SearchFreeSpace(rom, tbllen);
		if(ROMADDRESS_NULL == sa)
		{
			obs->err(0, GSID_ASSEMBLE_FREE_NOT_FOUND);
			return false;
		}
		obs->debug(0, GSID_SPRITES_TWEAKS_LOC, sa);
		write24(inf->giepyTableGrps[4], sa);

		dst = rom->GetSnesPtr(rom, sa);
		assert(dst);

		/* write tag header */
		memcpy(dst, "STAR", 4);
		write16(dst+4, (uint16)(tbllen-1));
		write16(dst+6, (uint16)((tbllen-1)^0xffff));
		dst += 8;
		sa += 8;

		/* write table data */
		AddTweaks(&dst, &sa, opi_tw, (uint8*)tweaks, inf->giepySpTweaksPtrAdrPtr);
	}

	/*--- calculate table length ---*/
	/* insert grp 1 (normal sprite init/main) */
	tbllen = 0;
	tbllen += CalcTableLength(opi_spr);
	tbllen <<= 3;
	if(0 == tbllen)
	{
		write24(inf->giepyTableGrps[0], 0);
	}
	else
	{
		uint8* dst;

		sa = rom->SearchFreeSpace(rom, tbllen);
		if(ROMADDRESS_NULL == sa)
		{
			obs->err(0, GSID_ASSEMBLE_FREE_NOT_FOUND);
			return false;
		}
		obs->debug(0, GSID_SPRITES_TABLE_GRP_LOC, 1, sa);
		write24(inf->giepyTableGrps[0], sa);

		dst = rom->GetSnesPtr(rom, sa);
		assert(dst);

		/* write tag header */
		memcpy(dst, "STAR", 4);
		write16(dst+4, (uint16)(tbllen-1));
		write16(dst+6, (uint16)((tbllen-1)^0xffff));
		dst += 8;
		sa += 8;

		/* write optimized table data */
		AddData(&dst, &sa, opi_spr, (uint8*)sprinit, inf->giepySpInitPtrAdrPtr);
		AddData(&dst, &sa, opi_spr, (uint8*)sprmain, inf->giepySpMainPtrAdrPtr);
	}

	/* insert grp 2(shooter generator main/initializer init) */
	tbllen = 0;
	tbllen += CalcTableLength(opi_shoot);
	tbllen += CalcTableLength(opi_gen);
	tbllen += CalcTableLength(opi_ini);
	tbllen <<= 2;
	if(0 == tbllen)
	{
		write24(inf->giepyTableGrps[1], 0);
	}
	else
	{
		uint8* dst;

		sa = rom->SearchFreeSpace(rom, tbllen);
		if(ROMADDRESS_NULL == sa)
		{
			obs->err(0, GSID_ASSEMBLE_FREE_NOT_FOUND);
			return false;
		}
		obs->debug(0, GSID_SPRITES_TABLE_GRP_LOC, 2, sa);
		write24(inf->giepyTableGrps[1], sa);

		dst = rom->GetSnesPtr(rom, sa);
		assert(dst);

		/* write tag header */
		memcpy(dst, "STAR", 4);
		write16(dst+4, (uint16)(tbllen-1));
		write16(dst+6, (uint16)((tbllen-1)^0xffff));
		dst += 8;
		sa += 8;

		/* write optimized table data */
		AddData(&dst, &sa, opi_shoot, (uint8*)shootmain, inf->giepyShootPtrAdrPtr);
		AddData(&dst, &sa, opi_gen, (uint8*)genmain, inf->giepyGenPtrAdrPtr);
		AddData(&dst, &sa, opi_ini, (uint8*)initinit, inf->giepyInitPtrAdrPtr);
	}

	/* insert grp 3(L1scroll init/main), 4(L2scroll init/main) */
	tbllen = 0;
	tbllen += CalcTableLength(opi_scr);
	tbllen <<= 3;
	if(0 == tbllen)
	{
		write24(inf->giepyTableGrps[2], 0);
		write24(inf->giepyTableGrps[3], 0);
	}
	else
	{
		uint8* dst;

		/*--- group 3 ---*/
		sa = rom->SearchFreeSpace(rom, tbllen);
		if(ROMADDRESS_NULL == sa)
		{
			obs->err(0, GSID_ASSEMBLE_FREE_NOT_FOUND);
			return false;
		}
		obs->debug(0, GSID_SPRITES_TABLE_GRP_LOC, 3, sa);
		write24(inf->giepyTableGrps[2], sa);

		dst = rom->GetSnesPtr(rom, sa);
		assert(dst);

		/* write tag header */
		memcpy(dst, "STAR", 4);
		write16(dst+4, (uint16)(tbllen-1));
		write16(dst+6, (uint16)((tbllen-1)^0xffff));
		dst += 8;
		sa += 8;

		/* write optimized table data */
		AddData(&dst, &sa, opi_scr, (uint8*)l1scrinit, inf->giepyL1ScrInitAdrPtr);
		AddData(&dst, &sa, opi_scr, (uint8*)l1scrmain, inf->giepyL1ScrMainAdrPtr);


		/*--- group 4 ---*/
		sa = rom->SearchFreeSpace(rom, tbllen);
		if(ROMADDRESS_NULL == sa)
		{
			obs->err(0, GSID_ASSEMBLE_FREE_NOT_FOUND);
			return false;
		}
		obs->debug(0, GSID_SPRITES_TABLE_GRP_LOC, 4, sa);
		write24(inf->giepyTableGrps[3], sa);

		dst = rom->GetSnesPtr(rom, sa);
		assert(dst);

		/* write tag header */
		memcpy(dst, "STAR", 4);
		write16(dst+4, (uint16)(tbllen-1));
		write16(dst+6, (uint16)((tbllen-1)^0xffff));
		dst += 8;
		sa += 8;

		/* write optimized table data */
		AddData(&dst, &sa, opi_scr, (uint8*)l2scrinit, inf->giepyL2ScrInitAdrPtr);
		AddData(&dst, &sa, opi_scr, (uint8*)l2scrmain, inf->giepyL2ScrMainAdrPtr);
	}

	if(inf->giepyIsExtBytesCode)
	{
		/* Fix sprite length */
		if(false == NormalizeSprDataLen(rom, (uint8*)inf->lmSpriteLengthBackup, inf->giepyExtBytesPtr, obs)) return false;
	}

	return true;
}

