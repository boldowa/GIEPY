/**
 * @file MewInterfaces.c
 *   ... These functions are exported to the outside.
 */
#include <bolib.h>
#include <assert.h>
#include <bolib/file/RomFile.h>
#include "jansson.h"
#include "common/version.h"
#include "common/strres.h"
#include "common/srcpath.h"
#include "common/Observer.h"
#include "mewthree/MewEnv.h"
#include "mewthree/CollectInfo.h"
#include "mewthree/Uninstall.h"
#include "mewthree/ChkAsmType.h"
#include "mewthree/ParseList.h"
#include "mewthree/ParseCfg.h"
#include "mewthree/Mewthree.h"
#include "common/dllCommon.h"
#include "dll/asardll.h"

#include "common/MewInterfaces.h"
#include "dll/MewthreeDll.h"

/* macros for export cross platform */
#if defined(CPPCLI)
#define Export 
#elif isWindows
#  define Export __declspec(dllexport)
#else
#  define Export __attribute__ ((visibility ("default")))
#endif

#define EVAL(f, v) f(v)
#define TO_STR_HELPER(s) #s
#define TO_STR(s) EVAL(TO_STR_HELPER, s)

#define ASARLIB_FILE SHARED_LIB_PREFIX "asar" SHARED_LIB_EXT


/*-------------------- prototypes --------------------*/
static void _RegisterObserver(MewInterface*, Observers);
static bool _OpenRomFile(MewInterface*, const char* path);
static bool _WriteRomFile(MewInterface*);
static void _CloseRomFile(MewInterface*);
static bool _UninstallAll(MewInterface*);
static bool _UninstallMain(MewInterface*);
static bool _CleanData(MewInterface*);
static AsmType _ChkAsmType(MewInterface*, const char* path);
static CfgData* _ParseCfg(MewInterface*, const char* path);
static CfgData* _ParseJson(MewInterface* m, const char* path);
static void _DiscardCfgData(MewInterface*, CfgData**);
static void _SetSystemPath(MewInterface*, const char*, const char*);
static void _ReleaseEnvironment(MewInterface*);
static bool _ParseList(MewInterface*, const char* listName);
static List* _GetInsertList(MewInterface*);
static void _InitializeWorkspace(MewInterface*, List*);
static bool _CollectLibraries(MewInterface*, const char*);
static bool _InitializeRom(MewInterface*, bool, bool, bool);
static bool _AssembleAll(MewInterface*, srcdirs* dir);
static bool _InsertSprite(MewInterface*, bool);
static bool _InsertExtended(MewInterface*);
static bool _InsertCluster(MewInterface*);
static bool _InsertOverworld(MewInterface*);
static bool _CheckDataLeak(MewInterface*);
static void _CleanWorkspace(MewInterface*);
static void _RoughTimer(MewInterface* m, uint msecs, uint interval);

/*--------------- Dir info ---------------*/
const char* base_dir = NULL;

/**
 * @brief Set the path of dll
 *
 * @param dir directory string
 */
Export void mew_SetBaseDir(const char* dir)
{
	base_dir = dir;
}

/*--------------- Asar ---------------*/

/**
 * @brief version info
 */
static char asar_version_str[16];
static char asar_api_version_str[16];

/**
 * @brief Inittialize asar library
 */
Export bool mew_InitAsar()
{
	int asar_ver_raw;
	int asar_api_ver_raw;
	char dllpath[MAX_PATH];

	sprintf_s(dllpath, MAX_PATH, "%s%s", base_dir, ASARLIB_FILE);
	if(!asar_init_with_dll_path(dllpath))
		return false;

	/*** get version ***/
	asar_ver_raw = asar_version();
	asar_api_ver_raw = asar_apiversion();
	sprintf_s(asar_version_str, 16, "%d.%d.%d",
			asar_ver_raw/10000,
			asar_ver_raw/100%100,
			asar_ver_raw%100);
	sprintf_s(asar_api_version_str, 16, "%d.%d",
			asar_api_ver_raw/100,
			asar_api_ver_raw%100);

	return true;
}

/**
 * @brief Close asar library
 */
Export void mew_CloseAsar()
{
	asar_close();
}

/**
 * @brief Get asar version string
 *
 * @return version string
 */
Export const char* mew_GetAsarVersion()
{
	return asar_version_str;
}
/**
 * @brief Get asar api version string
 *
 * @return version string
 */
Export const char* mew_GetAsarAPIVersion()
{
	return asar_api_version_str;
}

/*--------------- Jansson Version ---------------*/
static char* JanssonVersionStr = JANSSON_VERSION;
Export const char* mew_GetJanssonVersion()
{
	return JanssonVersionStr;
}

/*--------------- Version info ---------------*/
static const char* ver_str = TO_STR(DllAppVersion);

/**
 * @brief Get mewthree library version
 *
 * @return version value
 */
Export int mew_GetMewthreeVersion()
{
	return DllAppVersionInt;
}
/**
 * @brief Get mewthree library verison
 *
 * @return version string
 */
Export const char* mew_GetMewthreeVersionStr()
{
	return ver_str;
}

/*--------------- Create interface ---------------*/
/**
 * @brief Create mewthree interface object
 *
 * @return mewthree interface object
 */
Export MewInterface* mew_CreateMew()
{
	MewInterface* mew;
	Mewthree* m;

	/* create mewthree instance */
	m = new_Mewthree();
	if(NULL == m) return NULL;

	/* create mewthree interface */
	mew = calloc(1, sizeof(MewInterface));
	if(NULL == mew)
	{
		delete_Mewthree(&m);
		return NULL;
	}

	/* set interfaces */
	mew->instance = (void*)m;
	mew->RegisterObserver = _RegisterObserver;
	mew->OpenRomFile = _OpenRomFile;
	mew->WriteRomFile = _WriteRomFile;
	mew->CloseRomFile = _CloseRomFile;
	mew->UninstallAll = _UninstallAll;
	mew->UninstallMain = _UninstallMain;
	mew->CleanData = _CleanData;
	mew->ChkAsmType = _ChkAsmType;
	mew->ParseCfg = _ParseCfg;
	mew->ParseJson = _ParseJson;
	mew->DiscardCfgData = _DiscardCfgData;
	mew->SetSystemPath = _SetSystemPath;
	mew->ReleaseEnvironment = _ReleaseEnvironment;
	mew->ParseList = _ParseList;
	mew->GetInsertList = _GetInsertList;
	mew->InitializeWorkspace = _InitializeWorkspace;
	mew->CollectLibraries = _CollectLibraries;
	mew->InitializeRom = _InitializeRom;
	mew->AssembleAll = _AssembleAll;
	mew->InsertSprite = _InsertSprite;
	mew->InsertExtended = _InsertExtended;
	mew->InsertCluster = _InsertCluster;
	mew->InsertOverworld = _InsertOverworld;
	mew->CheckDataLeak = _CheckDataLeak;
	mew->CleanWorkspace = _CleanWorkspace;
	mew->RoughTimer = _RoughTimer;

	return mew;
}

/*--------------- Discard interface ---------------*/
/**
 * @brief Delete mewthree interface object
 *
 * @param mewp The pointer of mewthree interface object
 */
Export void mew_DiscardMew(MewInterface** mewp)
{
	MewInterface* mew;

	assert(mewp);
	mew = (*mewp);
	if(NULL == mew) return;

	delete_Mewthree((Mewthree**)&mew->instance);
	free(mew);
	(*mewp) = NULL;
}


/*-------------------- interface internal functions --------------------*/
#define assertMewMew(a, b) \
	Mewthree* b; \
	assert(a); \
	assert(a->instance); \
	b = (Mewthree*)a->instance;
/**
 * @brief Register observer 
 *
 * @param m mew interface object
 * @param obs observers
 */
static void _RegisterObserver(MewInterface* m, Observers obs)
{
	assertMewMew(m, mi);
	mi->RegisterObserver(mi, obs);
}

/**
 * @brief Open the ROM file for edit
 *
 * @param m mew interface object
 * @param path ROM path string
 *
 * @return ROM open succeeded / failure
 */
static bool _OpenRomFile(MewInterface* m, const char* path)
{
	assertMewMew(m, mi);
	return mi->OpenRomFile(mi, path);
}

/**
 * @brief Write edited data in the ROM file
 *
 * @param m mew interface object
 *
 * @return succeeded(true) / failure(false)
 */
static bool _WriteRomFile(MewInterface* m)
{
	assertMewMew(m, mi);
	return mi->WriteRomFile(mi);
}
/**
 * @brief Close the ROM file
 *
 * @param m mew interface object
 */
static void _CloseRomFile(MewInterface* m)
{
	assertMewMew(m, mi);
	mi->CloseRomFile(mi);
}
/**
 * @brief Uninstall all of data from ROM image
 *
 * @param m mew interface object
 *
 * @return succeeded(true) / failure(false)
 */
static bool _UninstallAll(MewInterface* m)
{
	assertMewMew(m, mi);
	return mi->UninstallAll(mi);
}
/**
 * @brief Uninstall program system data from ROM image
 *
 * @param m mew interface object
 *
 * @return succeeded(true) / failure(false)
 */
static bool _UninstallMain(MewInterface* m)
{
	assertMewMew(m, mi);
	return mi->UninstallMain(mi);
}
/**
 * @brief Clean sprite and library data
 *
 * @param m mew interface object
 *
 * @return succeeded(true) / failure(false)
 */
static bool _CleanData(MewInterface* m)
{
	assertMewMew(m, mi);
	return mi->Clean(mi);
}
/**
 * @brief Check type of asm
 *
 * @param m mew interface object
 * @param path path of target asm
 *
 * @return asm type value
 */
static AsmType _ChkAsmType(MewInterface* m, const char* path)
{
	assertMewMew(m, mi);
	return mi->ChkAsmType(mi, path);
}

/**
 * @brief Load sprite config data
 *
 * @param m mew interface object
 * @param path Config file path of sprite
 *
 * @return Read data. It returns NULL if failure.
 */
static CfgData* _ParseCfg(MewInterface* m, const char* path)
{
	assertMewMew(m, mi);
	return mi->ParseCfg(mi, path);
}

/**
 * @brief Load sprite config data(Json version)
 *
 * @param m mew interface object
 * @param path Json file path of sprite
 *
 * @return Read data. It returns NULL if failure.
 */
static CfgData* _ParseJson(MewInterface* m, const char* path)
{
	assertMewMew(m, mi);
	return mi->ParseJson(mi, path);
}

/**
 * @brief Discard sprite config data
 *
 * @param m mew interface object
 * @param d
 */
static void _DiscardCfgData(MewInterface* m, CfgData** d)
{
	assertMewMew(m, mi);
	mi->DiscardCfgData(mi, d);
}

/**
 * @brief Set current directory and exe path into environment
 *
 * @param m mew interface object
 * @param cur current directory path
 * @param exe exe directory path
 */
static void _SetSystemPath(MewInterface* m, const char* cur, const char* exe)
{
	assertMewMew(m, mi);
	mi->SetSystemPath(mi, cur, exe);
}

/**
 * @brief Release environment RAM.
 *
 * @param m mew interface object
 */
static void _ReleaseEnvironment(MewInterface* m)
{
	assertMewMew(m, mi);
	mi->ReleaseEnvironment(mi);
}

/**
 * @brief Parse list of sprite insertion
 *
 * @param m mew interface object
 * @param listName sprite list name (or path)
 *
 * @return succeeded(true) / failure(false)
 */
static bool _ParseList(MewInterface* m, const char* listName)
{
	assertMewMew(m, mi);
	return mi->ParseList(mi, listName);
}

/**
 * @brief Get insert item list
 *
 * @param m mew interface object
 *
 * @return List of InsertItem structure(common/InsertList.h).
 */
static List* _GetInsertList(MewInterface* m)
{
	assertMewMew(m, mi);
	return mi->GetInsertList(mi);
}

/**
 * @brief Initialize workspace
 *
 * @param m mew interface object
 * @param defs
 */
static void _InitializeWorkspace(MewInterface* m, List* defs)
{
	assertMewMew(m, mi);
	mi->InitializeWorkspace(mi, defs);
}

/**
 * @brief Collect library information (e.g. label, file path)
 *
 * @param m mew interface object
 * @param dir
 *
 * @return succeeded(true) / failure(false)
 */
static bool _CollectLibraries(MewInterface* m, const char* dir)
{
	assertMewMew(m, mi);
	return mi->CollectLibraries(mi, dir);
}

/**
 * @brief Initialize ROM for GIEPY
 *
 * @param m mew interface object
 * @param exb Whether to enable extra bytes feature
 * @param force Whether to enable system forced reinstallation or not
 * @param pixi Whether to enable pixi compatible or not
 *
 * @return succeeded(true) / failure(false)
 */
static bool _InitializeRom(MewInterface* m, bool exb, bool force, bool pixi)
{
	assertMewMew(m, mi);
	return mi->InitializeRom(mi, exb, force, pixi);
}

/**
 * @brief It assembles all ASM files.
 *
 * @param m mew interface object
 * @param dirs All directory paths where the assembly file exists
 *
 * @return succeeded(true) / failure(false)
 */
static bool _AssembleAll(MewInterface* m, srcdirs* dirs)
{
	assertMewMew(m, mi);
	return mi->AssembleAll(mi, dirs);
}

/**
 * @brief Insert information about sprites(tweaks, subroutine address etc...)
 *
 * @param m mew interface object
 * @param disableSscGen 
 *
 * @return succeeded(true) / failure(false)
 */
static bool _InsertSprite(MewInterface* m, bool disableSscGen)
{
	assertMewMew(m, mi);
	return mi->InsertSprite(mi, disableSscGen);
}

/**
 * @brief Insert information about extended sprites.
 *
 * @param m mew interface object
 *
 * @return succeeded(true) / failure(false)
 */
static bool _InsertExtended(MewInterface* m)
{
	assertMewMew(m, mi);
	return mi->InsertExtended(mi);
}

/**
 * @brief Insert information about cluster sprites.
 *
 * @param m mew interface object
 *
 * @return succeeded(true) / failure(false)
 */
static bool _InsertCluster(MewInterface* m)
{
	assertMewMew(m, mi);
	return mi->InsertCluster(mi);
}

/**
 * @brief Insert information about Overworld sprites.
 *
 * @param m mew interface object
 *
 * @return succeeded(true) / failure(false)
 */
static bool _InsertOverworld(MewInterface* m)
{
	assertMewMew(m, mi);
	return mi->InsertOverworld(mi);
}

/**
 * @brief It checks whether there is no data not inserted
 *
 * @param m mew interface object
 *
 * @return succeeded(true) / failure(false)
 */
static bool _CheckDataLeak(MewInterface* m)
{
	assertMewMew(m, mi);
	return mi->CheckDataLeak(mi);
}

/**
 * @brief Cleanup for workspace
 *
 * @param m mew interface object
 */
static void _CleanWorkspace(MewInterface* m)
{
	assertMewMew(m, mi);
	mi->CleanWorkspace(mi);
}

/**
 * @brief 
 *
 * @param m mew interface object
 * @param msecs Total count time (milliseconds)
 * @param interval Notification interval(milliseconds)
 */
static void _RoughTimer(MewInterface* m, uint msecs, uint interval)
{
	assertMewMew(m, mi);
	mi->RoughTimer(mi,msecs,interval);
}

