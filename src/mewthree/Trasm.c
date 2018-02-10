/**
 * @file Trasm.c
 */
#include "common/types.h"
#include <assert.h>
#include <setjmp.h>
#include "common/Str.h"
#include "common/ReadWrite.h"
#include "common/InsertList.h"
#include "common/strres.h"
#include "common/Observer.h"
#include "common/Funex.h"
#include "common/srcpath.h"
#include "common/dllCommon.h"
#include "common/PipeMan.h"
#include "file/FilePath.h"
#include "file/File.h"
#include "file/TextFile.h"
#include "file/RomFile.h"
#include "file/libfile.h"
#include "mewthree/Signature.h"
#include "mewthree/RomMap.h"
#include "mewthree/Rats.h"
#include "mewthree/MewEnv.h"
#include "mewthree/InsInfo.h"
#include "mewthree/Workspace.h"
#include "mewthree/LibsInsertMan.h"
#include "mewthree/Libraries.h"

#include "mewthree/Trasm.h"

#define NPOS 0xffffffff

/**
 * @brief components directory
 */
extern const char* base_dir;

/**
 * @brief remove the trailing spaces of the input string
 *
 * @param s string
 */
static void chomp(char* s)
{
	size_t i;

	i = strlen(s)-1;
	for(;0<=i;i--)
	{
		if(!IsSpace(s[i])) break;
		s[i] = '\0';
	}
}

/**
 * @brief search binary data
 *
 * @param p1 target data
 * @param p2 search data
 * @param len search data length
 * @param end data end pointer
 *
 * @return If data is found, it returns a pointer of the found location.
 *         If it can't be found, it returns NULL.
 */
static uint32 SearchData(const uint8* p1, const uint8* p2, int32 len, const uint8* end)
{
	int32 tail;
	int32 shifts[256];
	int i;
	const uint8* beg = p1;

	/* generate shift table */
	for(i=0; i<256; i++) shifts[i] = len;
	for(i=len-1;i>=0;i--)
	{
		shifts[p2[i]] = len-i-1;
	}

	/* search */
	tail = len-1;
	while(&p1[tail] <= end)
	{
		/* skip until tail match */
		i = tail;
		if(p1[i] != p2[i])
		{
			if(0 != shifts[p1[i]])
			{
				p1 = p1 + shifts[p1[i]];
			}
			else
			{
				p1++;
			}
			continue;
		}

		/* search from tail */
		for(i--; i>=0; i--)
		{
			if(p1[i] != p2[i]) break;
		}

		/* match all data, found */
		if(i<0) return (uint32)(p1-beg);

		/* not match, increase pointer */
		if(0 != shifts[p1[i+1]])
		{
			p1 = p1 + shifts[p1[i+1]];
		}
		else
		{
			p1++;
		}
	}
	return NPOS;
}

/**
 * @brief read binary data into buffer
 *
 * @param path binary file path
 * @param size binary file size
 * @param obs Observer
 *
 * @return the read buffer. If reading fails, NULL is returned.
 */
static uint8* readbin(const char* path, uint32* size, Observers* obs)
{
	FILE *fp;
	long sz;
	uint8* bin;

	sz = getfilesize(path);
	if((SpSigLen+5) > sz)
	{
		obs->err(0, GSID_ASSEMBLE_EMPTY_ASM);
		return NULL;
	}

	bin = malloc(sizeof(uint8)*(uint)sz);
	assert(bin);

	fp = fopen(path, "rb");
	if(NULL == fp)
	{
		obs->err(0, GSID_ASSEMBLE_BIN_OPEN_ERROR);
		return NULL;
	}

	if(sz != fread(bin, sizeof(uint8), (size_t)sz, fp))
	{
		fclose(fp);
		free(bin);
		return NULL;
	}
	fclose(fp);

	(*size) = (uint32)sz;
	return bin;
}

#ifdef CPPUTEST
  void errFree(void* d){free(d);}
#else
#  define errFree free
#endif
/**
 * @brief Assemble subroutine.
 *
 * @param orgaddr origin address
 * @param env Environment
 * @param obs Observer
 * @param cmd trasm command line arguments
 * @param tmpAsm temporary asm file object
 * @param path source code path
 * @param rom ROM file object
 * @param map RomMap information
 * @param siginj The function pointer for putout signature etc.
 * @param data arbitary data. GIEPY uses it for generate sprite number define.
 */
static bool AssembleSub(
	int orgaddr,
	MewEnvStruct* env,
	Observers* obs,
	const char* const cmd,
	TextFile* tmpAsm,
	const char* const path,
	RomFile* rom,
	RomMap* map,
	bool (*siginj)(TextFile*, void*),
	void* data)
{
	char* tmpPath;
	bool result = true;
	PipeMan* pman;
	const char* linebuf;
	char* work;

	/* open base asm */
	if(FileOpen_NoError != tmpAsm->Open2(tmpAsm, "w"))
	{
		return false;
	}

	/* write rommap inf */
	tmpAsm->Printf(tmpAsm, ";%s\n", map->name);
	tmpAsm->Printf(tmpAsm, "map = %d\n", map->val);

	/* write label / define src include */
	tmpPath = Str_concat(env->WorkDir, TrasmLibAsm);
	tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
	free(tmpPath);
	tmpPath = Str_concat(env->WorkDir, TrasmDefAsm);
	tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
	free(tmpPath);

	/* include rommap define file */
	if(MapMode_SA1 == rom->mapmode_get(rom))
	{
		tmpPath = Str_concat(env->ExeDir, TrasmSA1Def);
	}
	else
	{
		tmpPath = Str_concat(env->ExeDir, TrasmLoDef);
	}
	tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
	free(tmpPath);

	/* write base address */
	tmpAsm->Printf(tmpAsm, "org $%06x\n", orgaddr);

	/* write injection define */
	if(NULL != siginj) siginj(tmpAsm, data);

	/* write trasm include */
	tmpPath = Str_concat(env->ListDir, TrasmInc);
	if(fexists(tmpPath))
	{
		tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
	}
	else
	{
		free(tmpPath);
		tmpPath = Str_concat(env->ExeDir, TrasmIncSys);
		if(fexists(tmpPath)) tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", tmpPath);
	}
	free(tmpPath);

	/* write main src include */
	tmpAsm->Printf(tmpAsm, "incsrc \"%s\"\n", path);

	tmpAsm->super.Close(&tmpAsm->super);

	/* execute trasm */
	pman = new_PipeMan(cmd);
	if(NULL == pman)
	{
		obs->err(0, GSID_ASSEMBLE_PIPE_OPEN_ERROR, __func__);
		return false;
	}

	/*----- collect error -----*/
	/* search pass3 step */
	linebuf = pman->readline(pman);
	while(NULL != linebuf)
	{
		if(0 == strncmp("Pass 3", linebuf, 6)) break;
		linebuf = pman->readline(pman);
	}
	/* return false when text getting fails */
	if(NULL == linebuf)
	{
		obs->err(0, GSID_ASSEMBLE_TRASM_ERR_COL);
		delete_PipeMan(&pman);
		return false;
	}

	/* collect pass3 error */
	while(NULL != linebuf)
	{
		work = Str_copy(linebuf);
		chomp(work);

		if('\0'==work[0])
		{
			/* empty line */
			free(work);
			linebuf = pman->readline(pman);
			continue;
		}

		if(0 == strncmp("Source code", work, 11)) break; /* termination */

		/* detect error, putout */
		if(0 == strncmp("Error", work, 5))
		{
			result = false;
			obs->err(0, GSID_ASSEMBLE_STR_WITH_ASMNAME, path, work);
		}
		free(work);
		linebuf = pman->readline(pman);
	}
	delete_PipeMan(&pman);

	return result;
}

/**
 * @brief Assemble the source code using trasm(trasm_b)
 *
 * @param env Environment
 * @param obs Observer
 * @param path source code path
 * @param tmpname temporary file name. main source is included from this asm.
 * @param rom ROM file object
 * @param map RomMap information
 * @param inf Insert information
 * @param siginj The function pointer for putout signature etc.
 * @param data arbitary data. GIEPY uses it for generate sprite number define.
 */
bool AssembleTrasm(
	MewEnvStruct* env,
	Observers* obs,
	const char* const path,
	const char* tmpname,
	RomFile* rom,
	RomMap* map,
	InsertInf *inf,
	bool (*siginj)(TextFile*, void*),
	void* data)
{
	jmp_buf e;
	char* tmpPath;
	TextFile* tmpAsm;
	FilePath* binPath;
	char cmd[512];
	uint8* bin = NULL;
	uint32 size;
	uint32 mainloc = NPOS;
	uint32 initloc = NPOS;
	uint32 sa = 0;
	uint8* ptr;

	/* check rom type */
	if(MT_LoRom != map->val)
	{
		/* trasm supports only LoROM */
		obs->warn(0, GSID_ASSEMBLE_TRASM_LO_ONLY);
		/* return false; */
	}

	/* create base asm object */
	tmpPath = Str_concat(env->WorkDir, tmpname);
	tmpAsm = new_TextFile(tmpPath);
	binPath = new_FilePath(tmpPath);
	/* create command string for trasm */
	sprintf_s(cmd, 512, "%s" TRASMEXE_FILE " -f  \"%s\"", base_dir, tmpPath);
	free(tmpPath);

	if(0 == setjmp(e))
	{
		/* object check */
		if((NULL == tmpAsm) || (NULL == binPath))
		{
			longjmp(e, 1);
		}

		/* set tmp binary file path */
		binPath->ext_set(binPath, ".bin");

		obs->debug(0, GSID_ASSEMBLE_FILE, path);

		/*** Pass 1 ***/
		if(false == AssembleSub(
				0x008000,
				env, obs, cmd,
				tmpAsm, path,
				rom, map, 
				siginj,
				data))
		{
			longjmp(e, 1);
		}

		/* get binary data */
		bin = readbin(binPath->path_get(binPath), &size, obs);
		if(NULL == bin)
		{
			longjmp(e, 1);
		}

		/* get Init/Main location */
		initloc = SearchData(bin, (const uint8*)"INIT", 4, bin+size-1);
		mainloc = SearchData(bin, (const uint8*)"MAIN", 4, bin+size-1);

		if((NPOS == initloc) && (NPOS == mainloc))
		{
			obs->warn(0, GSID_ASSEMBLE_ENTRY_NOT_FOUND);
			initloc = mainloc = 0;
		}
		else
		{
			if(NPOS == initloc)
			{
				initloc = mainloc;
			}
			else if(NPOS == mainloc)
			{
				mainloc = initloc;
			}
		}
		free(bin);

		/* search free space */
		sa = rom->SearchFreeSpace(rom, size);
		if((ROMADDRESS_NULL == sa) || (NULL == rom->GetSnesPtr(rom, sa)))
		{
			obs->err(0, GSID_ASSEMBLE_FREE_NOT_FOUND);
			longjmp(e, 1);
		}

		/*** Pass 2 ***/
		if(false == AssembleSub(
				(int)sa+8,
				env, obs, cmd,
				tmpAsm, path,
				rom, map, 
				siginj,
				data))
		{
			longjmp(e, 1);
		}

		/*** copy data ***/
		ptr = rom->GetSnesPtr(rom, sa);
		assert(ptr);

		/* get data */
		bin = readbin(binPath->path_get(binPath), &size, obs);

		/* write data */
		WriteRatsData(ptr, bin, (uint)size);
		free(bin);

		/* add inserted file list */
		if(NULL != inf)
		{
			inf->path = Str_copy(path);
			inf->insloc = sa;
			inf->initloc = sa+initloc+8+4;	/* increase rats tag + "INIT" sig */
			inf->mainloc = sa+mainloc+8+4;	/* increase rats tag + "MAIN" sig */
		}
	}
	else
	{
		delete_TextFile(&tmpAsm);
		delete_FilePath(&binPath);
		obs->err(0, GSID_ASSEMBLE_FAILED, path);
		return false;
	}

	/* clean data */
#ifndef DEBUG
	remove(tmpAsm->super.path_get(&tmpAsm->super));
	remove(binPath->path_get(binPath));
#endif
	delete_TextFile(&tmpAsm);
	delete_FilePath(&binPath);
	return true;
}

