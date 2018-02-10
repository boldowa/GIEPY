/**
 * @file Libraries.c
 */
#include "common/types.h"
#include <setjmp.h>
#include "common/List.h"
#include "common/Str.h"
#include "common/Observer.h"
#include "common/InsertList.h"
#include "file/FilePath.h"
#include "file/File.h"
#include "file/TextFile.h"
#include "file/RomFile.h"
#include "dll/asardll.h"
#include "mewthree/RomMap.h"
#include "mewthree/Signature.h"
#include "mewthree/MewEnv.h"
#include "mewthree/LibsInsertMan.h"
#include "mewthree/Asar.h"
#include "mewthree/Export.h"

#include "mewthree/Libraries.h"

static bool sigInjection(TextFile* f, void* d)
{
	f->Printf(f, "!DEBUG := 0\n");
	f->Printf(f, "!SYSTEM_INITIALIZING := 0\n");
	f->Printf(f, "!LIBRARY_ASSEMBLING := 1\n");
	f->Printf(f, "freecode\n");
	f->Printf(f, "db " Signature_db "\n");
	f->Printf(f, "db \"LIB_\",$00,$00\n");
	return true;
}

static bool labelInjection(const struct labeldata* labs, int cnt, InsertInf* inf, void* data, MewEnvStruct* env, Observers* obs)
{
	const char* const skips[] = {
		":pos_",	/* + label  */
		":neg_",	/* - label  */
		"_",		/* internal */
		NULL
	};
	ExportLabels(labs, cnt, skips, env, obs);
	ExportFileMacro((char*)data, asar_getlabelval("__main__"), env, obs);
	return true;
}

bool InsertLibraries(
		RomFile* rom,
		RomMap* map,
		bool isPixiCompatible,
		int pass,
		List* labs,
		List* macros,
		LibsInsertMan* lman,
		MewEnvStruct* env,
		Observers* obs)
{
	Iterator *it;
	LibraryFileItem* fitem;
	bool result;
	const char* label;
	const char* macro;
	FilePath* fp;

	/* insert label libraries */
	for(it=labs->begin(labs); NULL != it; it=it->next(it))
	{
		label = (const char*)it->data(it);
		fitem = lman->searchLibrary(lman, label);

		/* label not found */
		if(NULL == fitem)
		{
			continue;
		}

		/* already inserted check */
		if(fitem->isInserted)
		{
			continue;
		}

		fp = new_FilePath(fitem->libpath);
		result = AssembleAsar(
					env, obs, isPixiCompatible,
					pass,
					fitem->libpath, "tmplib.asm",
					rom, map, lman,
					NULL,		/* inslist */
					sigInjection,	/* siginj */
					NULL,		/* printinj */
					labelInjection,	/* labinj */
					(char*)fp->name_get(fp));
		delete_FilePath(&fp);
		if(false == result)
		{
			return false;
		}

		fitem->isInserted = true;
	}

	/* insert macro libraries */
	if(isPixiCompatible)
	{
		for(it=macros->begin(macros); NULL != it; it=it->next(it))
		{
			macro = (const char*)it->data(it);
			fitem = lman->searchFilename(lman, macro);

			/* label not found */
			if(NULL == fitem)
			{
				continue;
			}

			/* already inserted check */
			if(fitem->isInserted)
			{
				continue;
			}

			fp = new_FilePath(fitem->libpath);
			result = AssembleAsar(
						env, obs, isPixiCompatible,
						pass,
						fitem->libpath, "tmplib.asm",
						rom, map, lman,
						NULL,		/* inslist */
						sigInjection,	/* siginj */
						NULL,		/* printinj */
						labelInjection,	/* labinj */
						(char*)fp->name_get(fp));
			delete_FilePath(&fp);
			if(false == result)
			{
				return false;
			}

			fitem->isInserted = true;
		}
	}

	return true;
}
