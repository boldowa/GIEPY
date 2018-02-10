/**
 * @file InitializeRom.c
 */
#include "common/types.h"
#include <assert.h>
#include "common/Str.h"
#include "common/List.h"
#include "common/strres.h"
#include "common/srcpath.h"
#include "common/InsertList.h"
#include "file/File.h"
#include "file/RomFile.h"
#include "file/TextFile.h"
#include "common/version.h"
#include "common/Observer.h"
#include "mewthree/Signature.h"
#include "mewthree/MewEnv.h"
#include "mewthree/RomMap.h"
#include "mewthree/CollectInfo.h"
#include "mewthree/InsInfo.h"
#include "mewthree/Uninstall.h"
#include "mewthree/LibsInsertMan.h"
#include "mewthree/Exbytes.h"
#include "dll/Mewthree.def"
#include "dll/asardll.h"
#include "mewthree/Asar.h"

#include "mewthree/InitializeRom.h"

static bool sigInjection(TextFile* f, void* d)
{
	f->Printf(f, "!DEBUG := 0\n");
	f->Printf(f, "!SYSTEM_INITIALIZING := 1\n");
	f->Printf(f, "!LIBRARY_ASSEMBLING := 0\n");
	f->Printf(f, "freecode\n");
	f->Printf(f, "db " Signature_db "\n");
	f->Printf(f, "db \"MAIN\",$00,$00\n");
	return true;
}

static bool Install(RomFile* rom, RomMap* map, LibsInsertMan* lman, MewInsInfo* inf, bool exb, bool pixi, MewEnvStruct* env, Observers* obs)
{
	char* maincodepath;
	bool result;

	maincodepath = Str_concat(env->ExeDir, SystemAsm);
	assert(maincodepath);

	if(exb)
	{
		uint8* p;

		if(1.80 > inf->lmVersion)
		{
			obs->err(0, GSID_INITIALIZE_EXB_LMVER_SMALL);
			return false;
		}
		else
		{
			p = rom->GetSnesPtr(rom, 0x0ef30f);
			if(NULL == p)
			{
				return false;
			}

			/* enable extra bytes */
			(*p) = 0x42;
		}
	}

	result = AssembleAsar(
				env, obs, pixi,
				1,
				maincodepath, "tmpmain.asm",
				rom, map,
				lman,
				NULL,
				sigInjection,
				NULL,
				NULL,
				NULL);

	free(maincodepath);
	return result;
}

bool InitializeRom(RomFile* rom, RomMap* map, LibsInsertMan* lman, MewInsInfo* inf, bool exb, bool force, bool pixi, MewEnvStruct* env, Observers* obs)
{
	uint8 bk[256*4];
	bool needToRestore = false;

	if(inf->giepyIsInstalled)
	{
		if((CodeVersion != inf->giepyCodeVer) || force)
		{

			if(CodeVersion < inf->giepyCodeVer)
			{
				obs->err(0, GSID_INITIALIZE_OLD_TOOL);
				return false;
			}

			if(force) obs->inf(0, GSID_INITIALIZE_FORCE);
			else      obs->inf(0, GSID_INITIALIZE_UPDATING);

			/* ask extra bytes info discard */
			if(inf->giepyIsExtBytesCode && !exb)
			{
				uint ans;
				ans = obs->ques(0, GSID_INITIALIZE_EXBYTE_QUES);
				if(MEW_OBS_NO == ans)
				{
					obs->inf(0, GSID_INITIALIZE_TERMED);
					return false;
				}
			}
			else
			{
				if(inf->giepyIsExtBytesCode)
				{
					memcpy(bk, inf->giepyExtBytesPtr, 256*4);
					needToRestore = true;
				}
			}

			if(false == UninstallMainData(rom, inf, obs))
			{
				obs->err(0, GSID_INITIALIZE_UPDATE_NG);
				return false;
			}
			if(!exb && (inf->giepyIsExtBytesCode))
			{
				uint8 res[256*4];
				memset(res, 3, 256*4);
				NormalizeSprDataLen(rom, (uint8*)inf->lmSpriteLengthBackup, res, obs);
			}

			if(false == Install(rom, map, lman, inf, exb, pixi, env, obs))
			{
				obs->err(0, GSID_INITIALIZE_UPDATE_NG);
				return false;
			}
		}
	}
	else
	{
		obs->inf(0, GSID_INITIALIZE_INSTALLING);
		if(false == Install(rom, map, lman, inf, exb, pixi, env, obs))
		{
			return false;
		}
	}

	if(false == CollectInfo(rom, inf, obs) || CodeVersion != inf->giepyCodeVer)
	{
		obs->err(0, GSID_INITIALIZE_VER_NOT_MATCH);
		return false;
	}

	if(needToRestore)
	{
		memcpy(inf->giepyExtBytesPtr, bk, 256*4);
		memcpy(inf->lmSpriteLengthBackup, bk, 256*4);
	}

	return true;
}
