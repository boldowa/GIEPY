/**
 * @file Mewthree.c
 *   ... mewthree.dll main instance source
 */
#include <bolib.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#if isWindows
#  include <windows.h>
#  define ms_sleep(sec) Sleep(sec)
#else
#  ifndef __USE_XOPEN_EXTENDED
#    define __USE_XOPEN_EXTENDED
#  endif
#  include <unistd.h>
#  define ms_sleep(sec) usleep(sec*1000)
#endif
#include <bolib/file/RomFile.h>
#include <bolib/file/TextFile.h>
#include "common/strres.h"
#include "common/Observer.h"
#include "common/srcpath.h"
#include "mewthree/MewEnv.h"
#include "mewthree/RomMap.h"
#include "mewthree/CollectInfo.h"
#include "mewthree/Uninstall.h"
#include "mewthree/ChkAsmType.h"
#include "mewthree/ParseList.h"
#include "mewthree/ParseCfg.h"
#include "mewthree/Json.h"
#include "mewthree/Workspace.h"
#include "mewthree/LibsInsertMan.h"
#include "mewthree/InitializeRom.h"
#include "common/MewInterfaces.h"
#include "mewthree/Mewthree.h"
#include "mewthree/Sprites.h"
#include "mewthree/ExtClsSpr.h"
#include "dll/MewthreeDll.h"
#include "dll/asardll.h"
#include "mewthree/Asar.h"
#include "mewthree/Export.h"
#include "mewthree/AssembleAll.h"

#if isWindows
#  ifdef SearchPath
#    undef SearchPath
#  endif
#endif

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "Mewthree.protected.h"

/* prototypes */
static void RegisterObserver(Mewthree*, Observers obs);
static bool OpenRomFile(Mewthree*, const char*);
static bool WriteRomFile(Mewthree*);
static void CloseRomFile(Mewthree*);
static bool UninstallAll(Mewthree*);
static bool UninstallMain(Mewthree*);
static bool Clean(Mewthree*);
static AsmType ChkAsm(Mewthree*, const char*);
static CfgData* _ParseCfg(Mewthree*, const char* path);
static CfgData* _ParseJson(Mewthree*, const char* path);
static void _DiscardCfgData(Mewthree* mi, CfgData** d);
static void _SetSystemPath(Mewthree*, const char*, const char*);
static void _ReleaseEnvironment(Mewthree*);
static bool _ParseList(Mewthree*, const char* listName);
static List* _GetInsertList(Mewthree*);
static void _InitializeWorkspace(Mewthree*, List*);
static bool _CollectLibraries(Mewthree*, const char*);
static bool _InitializeRom(Mewthree*, bool, bool, bool);
static bool _AssembleAll(Mewthree*, srcdirs* dirs);
static bool _InsertSprite(Mewthree*, bool disableSscGen);
static bool _InsertExtended(Mewthree*);
static bool _InsertCluster(Mewthree*);
static bool _InsertOverworld(Mewthree*);
static bool _CheckDataLeak(Mewthree*);
static void _CleanWorkspace(Mewthree*);
static void _RoughTimer(Mewthree* m, uint msecs, uint interval);

/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create Mewthree object
 *
 * @return the pointer of object
 */
Mewthree* new_Mewthree(void)
{
	Mewthree* self;
	Mewthree_protected* pro;

	/* make objects */
	self = malloc(sizeof(Mewthree));
	pro = malloc(sizeof(Mewthree_protected));

	/* check whether object creatin succeeded */
	assert(pro);
	assert(self);

	/*--- set protected member ---*/
	pro->rom = NULL;
	pro->inslist = NULL;
	pro->isSeparateGroup = false;
	memset(&pro->obs, 0, sizeof(Observers));
	memset(&pro->env, 0, sizeof(MewEnvStruct));
	memset(&pro->inf, 0, sizeof(MewInsInfo));
	memset(pro->inf.lmSpriteLengthBackup, 3, 256*4); /* default spr len: 3 */
	pro->libsMan = new_LibsInsertMan();
	assert(pro->libsMan);

	/*--- set public member ---*/
	self->RegisterObserver = RegisterObserver;
	self->OpenRomFile = OpenRomFile;
	self->WriteRomFile = WriteRomFile;
	self->CloseRomFile = CloseRomFile;
	self->UninstallAll = UninstallAll;
	self->UninstallMain = UninstallMain;
	self->Clean = Clean;
	self->ChkAsmType = ChkAsm;
	self->ParseCfg = _ParseCfg;
	self->ParseJson = _ParseJson;
	self->DiscardCfgData = _DiscardCfgData;
	self->SetSystemPath = _SetSystemPath;
	self->ReleaseEnvironment = _ReleaseEnvironment;
	self->ParseList = _ParseList;
	self->GetInsertList = _GetInsertList;
	self->InitializeWorkspace = _InitializeWorkspace;
	self->CollectLibraries = _CollectLibraries;
	self->InitializeRom = _InitializeRom;
	self->AssembleAll = _AssembleAll;
	self->InsertSprite = _InsertSprite;
	self->InsertExtended = _InsertExtended;
	self->InsertCluster = _InsertCluster;
	self->InsertOverworld = _InsertOverworld;
	self->CheckDataLeak = _CheckDataLeak;
	self->CleanWorkspace = _CleanWorkspace;
	self->RoughTimer = _RoughTimer;

	/* init Mewthree object */
	self->pro = pro;
	return self;
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_Mewthree_members(Mewthree* self)
{
	/* delete protected members */
	if(NULL != self->pro->rom)
	{
		self->pro->rom->close(self->pro->rom);
		delete_RomFile(&self->pro->rom);
	}
	delete_LibsInsertMan(&self->pro->libsMan);
	delete_List(&self->pro->inslist);
	ReleaseEnvironment(&self->pro->env);
	free(self->pro);
}

/**
 * @brief Delete Mewthree object
 *
 * @param the pointer of object
 */
void delete_Mewthree(Mewthree** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_Mewthree_members(*self);
	free(*self);
	(*self) = NULL;
}


/*--------------- internal methods ---------------*/

#define RegistObserver(dest, src, f) \
	dest.f = nopObserver; \
	if(src.f != NULL) dest.f = src.f
static uint nopObserver(uint flag, uint id, ...){ return MEW_OBS_OK; }
static void nopNotifier(uint x, uint y){}
static void RegisterObserver(Mewthree* mi, Observers obs)
{
	/* register functions */
	RegistObserver(mi->pro->obs, obs, debug);
	RegistObserver(mi->pro->obs, obs, inf);
	RegistObserver(mi->pro->obs, obs, ques);
	RegistObserver(mi->pro->obs, obs, warn);
	RegistObserver(mi->pro->obs, obs, err);
	RegistObserver(mi->pro->obs, obs, fatal);
	/* register notifier */
	mi->pro->obs.progress = nopNotifier;
	if(obs.progress != NULL) mi->pro->obs.progress = obs.progress;
}
#undef RegistObserver

static bool OpenRomFile(Mewthree* mi, const char* path)
{
	RomFile *rom;

	if(NULL != path)
	{
		if(NULL != mi->pro->rom)
		{
			delete_RomFile(&mi->pro->rom);
		}
		mi->pro->rom = new_RomFile(path);
	}

	rom = mi->pro->rom;
	if(NULL == rom)
	{
		mi->pro->obs.fatal(0, GSID_MEMALLOC_FAILED, __func__);
		return false;
	}

	/* open */
	if(FileOpen_NoError != rom->open(rom))
	{
		mi->pro->obs.err(0, GSID_ROM_OPEN_FAILED);
		delete_RomFile(&mi->pro->rom);
		return false;
	}

	/* Set path information */
	if(NULL != mi->pro->env.RomDir) free((char*)mi->pro->env.RomDir);
	mi->pro->env.RomDir = abspath(rom->super.dir_get(&rom->super));
	SetSearchPath(&mi->pro->env);
	/* set rom mapmode for assembling */
	getmapmode(rom, &mi->pro->map);

	/* collect giepy information */
	if(false == CollectInfo(rom, &mi->pro->inf, &mi->pro->obs))
	{
		delete_RomFile(&mi->pro->rom);
		return false;
	}

	return true;
}

static bool WriteRomFile(Mewthree* mi)
{
	return mi->pro->rom->write(mi->pro->rom);
}

static void CloseRomFile(Mewthree* mi)
{
	mi->pro->rom->close(mi->pro->rom);
}

static bool UninstallAll(Mewthree* mi)
{
	return UninstallAllData(mi->pro->rom, &mi->pro->inf, &mi->pro->obs);
}

static bool UninstallMain(Mewthree* mi)
{
	return UninstallMainData(mi->pro->rom, &mi->pro->inf, &mi->pro->obs);
}

static bool Clean(Mewthree* mi)
{
	return CleanData(mi->pro->rom, &mi->pro->inf, &mi->pro->obs);
}

static AsmType ChkAsm(Mewthree* mi, const char* path)
{
	return ChkAsmType(path, &mi->pro->obs);
}

static CfgData* _ParseCfg(Mewthree* mi, const char* path)
{
	return ParseCfg(path, 0, &mi->pro->obs);
}

static CfgData* _ParseJson(Mewthree* mi, const char* path)
{
	return ParseJson(path, 0, &mi->pro->obs);
}

static void _DiscardCfgData(Mewthree* mi, CfgData** d)
{
	DiscardCfgData(d, &mi->pro->obs);
}

static void _SetSystemPath(Mewthree* mi, const char* cur, const char* exe)
{
	SetSystemPath(&mi->pro->env, cur, exe, &mi->pro->obs);
}

static void _ReleaseEnvironment(Mewthree* mi)
{
	ReleaseEnvironment(&mi->pro->env);
}

static bool _ParseList(Mewthree* mi, const char* listName)
{
	return ParseList(listName, &mi->pro->inslist, &mi->pro->env, &mi->pro->obs);
}

static List* _GetInsertList(Mewthree* mi)
{
	return mi->pro->inslist;
}

static void _InitializeWorkspace(Mewthree* mi, List* defs)
{
	InitializeWorkspace(&mi->pro->env, &mi->pro->inf, defs);
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
	return true;
}
static bool sigInjection(TextFile* f, void* d)
{
	f->printf(f, "!DEBUG := 0\n");
	f->printf(f, "!SYSTEM_INITIALIZING := 1\n");
	f->printf(f, "!LIBRARY_ASSEMBLING := 0\n");
	return true;
}
static bool _CollectLibraries(Mewthree* mi, const char* dir)
{
	char* smwlibpath;
	bool result;

	/* search libraries */
	mi->pro->libsMan->buildData(mi->pro->libsMan, dir, &mi->pro->env, &mi->pro->obs);

	/* create smw libraries */
	smwlibpath = Str_concat(mi->pro->env.ExeDir, SmwLibAsm);
	assert(smwlibpath);
	result = AssembleAsar(
			&mi->pro->env, &mi->pro->obs, false,
			1, /* 1 pass */
			smwlibpath, "tmpsys.asm",
			mi->pro->rom, &mi->pro->map,
			mi->pro->libsMan,
			NULL,
			sigInjection,
			NULL,
			labelInjection,
			NULL);
	free(smwlibpath);
	return result;
}

static bool _InitializeRom(Mewthree* mi, bool exb, bool force, bool pixi)
{
	if(false == CleanData(mi->pro->rom, &mi->pro->inf, &mi->pro->obs)) return false;
	return InitializeRom(mi->pro->rom, &mi->pro->map, mi->pro->libsMan, &mi->pro->inf, exb, force, pixi, &mi->pro->env, &mi->pro->obs);
}

static bool _AssembleAll(Mewthree* mi, srcdirs* dirs)
{
	return AssembleAll(mi->pro->rom, &mi->pro->map, dirs, mi->pro->inslist, mi->pro->libsMan, &mi->pro->inf, &mi->pro->env, &mi->pro->obs);
}

static bool _InsertSprite(Mewthree* mi, bool disableSscGen)
{
	return InsertSprites(mi->pro->rom, &mi->pro->inf, mi->pro->inslist, disableSscGen, &mi->pro->env, &mi->pro->obs);
}

static bool _InsertExtended(Mewthree* mi)
{
	return InsertExtended(mi->pro->rom, &mi->pro->inf, mi->pro->inslist, &mi->pro->env, &mi->pro->obs);
}

static bool _InsertCluster(Mewthree* mi)
{
	return InsertCluster(mi->pro->rom, &mi->pro->inf, mi->pro->inslist, &mi->pro->env, &mi->pro->obs);
}

static bool _InsertOverworld(Mewthree* mi)
{
	/*---(nothing to do)---*/
	return true;
}

static bool _CheckDataLeak(Mewthree* mi)
{
	/* deta remain check */
	if(mi->pro->inslist->length(mi->pro->inslist))
	{
		return false;
	}
	return true;
}

static void _CleanWorkspace(Mewthree* mi)
{
	CleanWorkspace(&mi->pro->env);
}

static void _RoughTimer(Mewthree* mi, uint msecs, uint interval)
{
	uint cnt;
	uint stime = interval;;

	/* timer loop */
	for(cnt=0;cnt<msecs;cnt+=interval)
	{
		/* notify to parrent process */
		mi->pro->obs.progress(cnt, msecs);

		/* wait */
		if(msecs<(cnt+interval)) stime = msecs-cnt;
		ms_sleep(stime);
	}

	/* timer done. */
	mi->pro->obs.progress(msecs, msecs);
}
