/**
 * @file ExtClsSpr.c
 */
#include <bolib.h>
#include <assert.h>
#include <bolib/file/RomFile.h>
#include "common/strres.h"
#include "common/InsertList.h"
#include "common/Observer.h"
#include "common/CfgData.h"
#include "mewthree/Rats.h"
#include "mewthree/MewEnv.h"
#include "mewthree/InsInfo.h"

#include "mewthree/ExtClsSpr.h"

typedef struct _optinfo {
	int nums;
	int min;
	int max;
}opi;

bool InsertCommon(RomFile* rom, MewInsInfo* inf, List* iList, MewEnvStruct* env, Observers* obs, SpriteType stype, uint min, uint8* reloc)
{
	Iterator* it;
	Iterator* it_next;
	InsertItem* item;
	uint tbllen;
	uint32 sa;
	uint max = min;
	bool empty = true;
	uint8* relp;

	uint8 calltbl[256*3 + 4];

	/* get reloc pointer */
	sa = read24(reloc);
	relp = rom->GetSnesPtr(rom, sa);
	if(NULL == relp)
	{
		obs->err(0, GSID_EXTCLS_BROKEN_RELOC_ADR);
		return false;
	}

	sa = rom->Pc2SnesAdr(rom, 0x0006fa);	/* ExecutePtrLong Address */
	if(inf->giepyIsPixiCompatible)
	{
		sa = read24(inf->giepyExecutePtrLongPbAdrPtr);
	}
	memset(calltbl+4, 0x9b, 256*3);
	calltbl[0] = 0x22;		/* jsl */
	write24(&calltbl[1], sa);	/* ExecutePtrLong (or ExecutePtrLongPb) */

	/* data process */
	it = iList->begin(iList);

	while(NULL != it)
	{
		item = (InsertItem*)it->data(it);

		/* check sprite type */
		if(stype != item->type)
		{
			it = it->next(it);
			continue;
		}
		empty = false;

		if(max < item->number) max = (uint)item->number;
		write24(&calltbl[4+((item->number - (int)min)*3)], item->insinf.mainloc);

		/* delete */
		it_next = it->next(it);
		iList->remove(iList, it);
		it = it_next;
	}
	if(empty) return true;

	/*--- calculate table length ---*/
	tbllen = (max-min+1)*3 + 4;
	{
		uint8* dst;

		sa = rom->SearchFreeSpace(rom, tbllen);
		if(ROMADDRESS_NULL == sa)
		{
			obs->err(0, GSID_ASSEMBLE_FREE_NOT_FOUND);
			return false;
		}
		if(stype == SpriteType_Extended)
		{
			obs->debug(0, GSID_EXTCLS_EXTENDED_LOCATION, sa);
		}
		else
		{
			obs->debug(0, GSID_EXTCLS_CLUSTER_LOCATION, sa);
		}
		write24(relp, sa+8);
		dst = rom->GetSnesPtr(rom, sa);
		assert(dst);

		WriteRatsData(dst, calltbl, tbllen);
	}

	return true;
}

bool InsertExtended(RomFile* rom, MewInsInfo* inf, List* iList, MewEnvStruct* env, Observers* obs)
{
	return InsertCommon(rom, inf, iList, env, obs,
		SpriteType_Extended, 19, inf->giepyExtRelocAdrPtr);
}

bool InsertCluster(RomFile* rom, MewInsInfo* inf, List* iList, MewEnvStruct* env, Observers* obs)
{
	return InsertCommon(rom, inf, iList, env, obs,
		SpriteType_Cluster, 9, inf->giepyClsRelocAdrPtr);
}
