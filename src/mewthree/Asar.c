/**
 * @file Asar.c
 */
#include "common/types.h"
#include <assert.h>
#include <setjmp.h>
#include "common/Str.h"
#include "common/InsertList.h"
#include "common/strres.h"
#include "common/Observer.h"
#include "common/Funex.h"
#include "common/version.h"
#include "common/srcpath.h"
#include "file/File.h"
#include "file/TextFile.h"
#include "file/RomFile.h"
#include "file/libfile.h"
#include "mewthree/RomMap.h"
#include "mewthree/MewEnv.h"
#include "mewthree/InsInfo.h"
#include "mewthree/Workspace.h"
#include "mewthree/LibsInsertMan.h"
#include "mewthree/Libraries.h"
#include "dll/asardll.h"

#include "mewthree/Asar.h"

/**
 * @brief check if source has been inserted
 *
 * @param sval source path
 * @param lval list(List<InsertInf>) value
 *
 * @return true: already inserted / false: not match(search next item)
 */
bool InsertedChk(const void* sval, const void* lval)
{
	char* path;
	InsertInf* inf;

	path = (char*)sval;
	inf = (InsertInf*)lval;

	return (0 == strcmp(path, inf->path));
}
/**
 * @brief add missing macro to list
 *
 * @param macros missing label list
 * @param errline asar's error message line
 */
static void addErrorMacro(List* macros, const char* errline, const char* block)
{
	/**
	 * Error message match finder
	 *
	 *   Macro error format:
	 *     Unknown macro [%XXXXX()]
	 */

	jmp_buf e;
	char* work;
	char* macro;
	size_t i;
	size_t len;
	size_t mbeg;

	work = Str_copy(block);
	len = strlen(work);
	if(0 == setjmp(e))
	{
		if(0 != strncasecmp("Unknown macro", errline, 13)) longjmp(e, 1);

		i=0;
		SkipUntilChar(work, &i, '%', len);
		i++;
		if(len <= i) longjmp(e, 1);
		mbeg = i;

		SkipUntilChar(work, &i, '(', len);
		if(len <= i) longjmp(e, 1);
		work[i] = '\0';

		macro = Str_copy(&work[mbeg]);
		free(work);
		macros->push(macros, macro);
	}
	else
	{
		free(work);
	}
}

/**
 * @brief print asar error message
 *
 * @param obs observer
 */
static void putasarerr(Observers* obs)
{
	int nums;

	const struct errordata* err;
	err = asar_geterrors(&nums);

	for(;0<nums;nums--)
	{
		obs->err(0, GSID_STRING, (*err++).fullerrdata);
	}
}

/**
 * @brief add missing label to list
 *
 * @param labels missing label list
 * @param errline asar's error message line
 */
static void addErrorLabel(List* labels, const char* errline)
{
	/**
	 * Error message match finder
	 *
	 *   Label error format:
	 *     Label XXXXX not found
	 */

	jmp_buf e;
	char* work;
	char* lab;
	size_t i;
	size_t len;

	work = Str_copy(errline);
	len = strlen(work);
	if(0 == setjmp(e))
	{
		if(0 != strncasecmp("Label ", work, 6)) longjmp(e, 1);

		i=6;
		SkipUntilSpaces(work, &i, len);
		if(i == len) longjmp(e, 1);

		work[i] = '\0';
		if(0 != strncasecmp("not found", &work[i+1], 9)) longjmp(e, 1);

		lab = Str_copy(&work[6]);
		free(work);
		labels->push(labels, lab);
	}
	else
	{
		free(work);
	}
}

/**
 * @brief Assemble the source code using asar library
 *
 * @param env Environment
 * @param obs Observer
 * @param isPixiCompatible pixi
 * @param pass max compile path; retry for the number of times specified by this parameter
 * @param path source code path
 * @param tmpname temporary file name. main source is included from this asm.
 * @param rom ROM file object
 * @param map RomMap information
 * @param lman Library insert manager object
 * @param inf Insert information
 * @param siginj The function pointer for putout signature etc.
 * @param priinj The function pointer for analysys asar's print (for analysys 'print "INIT ",pc' etc.)
 * @param labinj The function pointer for export label info
 * @param data arbitary data. GIEPY uses it for generate sprite number define.
 */
bool AssembleAsar(
	MewEnvStruct* env,
	Observers* obs,
	bool isPixiCompatible,
	int pass,
	const char* const path,
	const char* tmpname,
	RomFile* rom,
	RomMap* map,
	LibsInsertMan* lman,
	InsertInf *inf,
	bool (*siginj)(TextFile*, void*),
	bool (*priinj)(const char*, InsertInf* inf, void* data, MewEnvStruct*, Observers*),
	bool (*labinj)(const struct labeldata*, int, InsertInf* inf, void* data, MewEnvStruct*, Observers*),
	void* data)
{
	jmp_buf e;
	char* tmpPath;
	TextFile* tmpAsm;
	bool result;
	int romlen = (int)rom->size_get(rom);

	/* create base asm object */
	tmpPath = Str_concat(env->WorkDir, tmpname);
	tmpAsm = new_TextFile(tmpPath);
	free(tmpPath);

	if(0 == setjmp(e))
	{
		/* object check */
		if(NULL == tmpAsm)
		{
			longjmp(e, 1);
		}

		/* open base asm */
		if(FileOpen_NoError != tmpAsm->Open2(tmpAsm, "w"))
		{
			longjmp(e, 1);
		}

		obs->debug(0, GSID_ASSEMBLE_FILE, path);

		/* write rommap inf */
		tmpAsm->Printf(tmpAsm, "%s\n", map->name);
		tmpAsm->Printf(tmpAsm, "!map = %d\n", map->val);

		/* write sa-1 config include */
		tmpPath = Str_concat(env->ExeDir, SA1Def);
		tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
		free(tmpPath);

		/* write injection define */
		if(NULL != siginj) siginj(tmpAsm, data);

		/* write sys define */
		tmpAsm->Printf(tmpAsm, "!SYSTEM_INITIALIZING ?= 0\n");
		tmpAsm->Printf(tmpAsm, "!GIEPY_ONLY ?= 0\n");
		tmpAsm->Printf(tmpAsm, "!GIEPY_VER := %06d\n", DllAppVersionInt);

		/* pixi inc */
		if(isPixiCompatible)
		{
			tmpAsm->Printf(tmpAsm, "!PIXI_COMPATIBLE = 1\n", tmpPath);
			tmpPath = Str_concat(env->ExeDir, PixiDef);
			tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
			free(tmpPath);
		}

		/*------------------------------------*/
		tmpAsm->Printf(tmpAsm, "pushpc\n");

		/* write label / define src include */
		tmpPath = Str_concat(env->WorkDir, AsarLibAsm);
		tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
		free(tmpPath);
		tmpPath = Str_concat(env->WorkDir, AsarDefAsm);
		tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
		free(tmpPath);
		tmpPath = Str_concat(env->WorkDir, AsarFNMacAsm);
		tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
		free(tmpPath);

		/* write asar include */
		tmpPath = Str_concat(env->ListDir, AsarInc);
		if(fexists(tmpPath))
		{
			tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
		}
		else
		{
			free(tmpPath);
			tmpPath = Str_concat(env->ExeDir, AsarIncSys);
			if(fexists(tmpPath)) tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
		}
		free(tmpPath);

		tmpAsm->Printf(tmpAsm, "pullpc\n");
		/*------------------------------------*/

		/* write main src include */
		tmpAsm->Printf(tmpAsm, "__main__:\n");
		tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", path);

		/* is giepy only sprite */
		tmpAsm->Printf(tmpAsm, "if 0 != !GIEPY_ONLY && !PIXI_COMPATIBLE\n");
		tmpAsm->Printf(tmpAsm, "print \"GIEPY ONLY SPRITE!!\"\n");
		tmpAsm->Printf(tmpAsm, "assert 0\n");
		tmpAsm->Printf(tmpAsm, "endif\n");

		/* write injection signature */
		/* if(NULL != sigput) sigput(tmpAsm, data); */

		tmpAsm->super.Close(&tmpAsm->super);

		/* Pass 1 */
		pass--;
		if(0>pass) pass = 0;
		asar_reset();
		result = asar_patch(
				tmpAsm->super.path_get(&tmpAsm->super),
				(char*)rom->GetPcPtr(rom,0),
				(int)rom->size_get(rom),
				&romlen);
		if((false == result) && (0<pass))
		{
#ifdef CPPUTEST
		void labFree(void* data){free(data);}
#else
#		define labFree free
#endif
			int i;
			int errnums;
			const struct errordata* err;
			List* labels = new_List(NULL, labFree);
			List* macros = new_List(NULL, labFree);
			assert(labels);

			/* Collect label not found error */
			err = asar_geterrors(&errnums);
			for(i=0; i<errnums; i++)
			{
				addErrorLabel(labels, err[i].rawerrdata);
				addErrorMacro(macros, err[i].rawerrdata, err[i].block);
			}

			/* Insert libraries */
			result = InsertLibraries(rom, map, isPixiCompatible, pass, labels, macros, lman, env, obs);
			delete_List(&labels);
			delete_List(&macros);

			/* Pass 2 */
			if(result)
			{
				asar_reset();
				result = asar_patch(
						tmpAsm->super.path_get(&tmpAsm->super),
						(char*)rom->GetPcPtr(rom,0),
						(int)rom->size_get(rom),
						&romlen);
			}
		}

		/* get prints */
		{
			int i;
			const char* const * s;
			s = asar_getprints(&i);

			if(NULL == priinj)
			{
				for(;0<i;i--) obs->inf(0, GSID_ASSEMBLE_STR_WITH_ASMNAME, path, (*s++));
			}
			else
			{
				for(;0<i;i--)
				{
					if(!priinj((*s), inf, data, env, obs))
					{
						obs->inf(0, GSID_ASSEMBLE_STR_WITH_ASMNAME, path, (*s));
					}
					s++;
				}
			}
		}
		/* hoge */

		/* check result */
		if(false == result)
		{
			putasarerr(obs);
			longjmp(e, 1);
		}

		/* label data process */
		if(NULL != labinj)
		{
			int i;
			const struct labeldata* l;
			l = asar_getalllabels(&i);
			if(false == labinj(l, i, inf, data, env, obs))
			{
				longjmp(e, 1);
			}
		}

		/* add inserted file list */
		if(NULL != inf)
		{
			int32 loc;
			loc = asar_getlabelval("__main__");
			if(-1 == loc)
			{
				longjmp(e, 1);
			}
			inf->path = Str_copy(path);
			inf->insloc = (uint32)loc;
		}
	}
	else
	{
		delete_TextFile(&tmpAsm);
		obs->err(0, GSID_ASSEMBLE_FAILED, path);
		return false;
	}

	/* clean data */
#ifndef DEBUG
	remove(tmpAsm->super.path_get(&tmpAsm->super));
#endif
	delete_TextFile(&tmpAsm);
	return true;
}

