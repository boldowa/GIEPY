/**
 * @file IniMan.c
 * Singleton ini manager
 */
#include <bolib.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include "common/Funex.h"
#include "common/defaults.h"
#include "common/defines.h"
#include "common/Environment.h"

#include "win32/IniMan.h"

#define IniName _T("piee.ini")

/* Private member */
struct _IniMan_private {
	/* System */
	INT X;
	INT Y;

	/* Insert Options */
	BOOL isForce;
	BOOL isExtraBytes;
	BOOL isPixiCompatible;
	BOOL disableSscGen;

	/* info option */
	BOOL isDebug;
	BOOL isDisableWarn;

	/* defines options */
	INT definesListNums;
	DefineListItem** definesLists;

	/* dir options */
	TCHAR* librariesDir;
	TCHAR* spritesDir;
	TCHAR* extendedesDir;
	TCHAR* clustersDir;

	/* mru options */
	INT recentRomNums;
	INT recentListNums;
	TCHAR** recentRomPaths;
	TCHAR** recentListPaths;
};

/* prototypes */
static bool loadIni(void);
static bool saveIni(void);
static BOOL get_isForce(void);
static void set_isForce(BOOL);
static BOOL get_isExtraBytes(void);
static void set_isExtraBytes(BOOL);
static BOOL get_isPixiCompatible(void);
static void set_isPixiCompatible(BOOL);
static BOOL get_disableSscGen(void);
static void set_disableSscGen(BOOL);
static BOOL get_isDebug(void);
static void set_isDebug(BOOL);
static BOOL get_isDisableWarn(void);
static void set_isDisableWarn(BOOL);
static INT  get_X(void);
static void set_X(INT);
static INT  get_Y(void);
static void set_Y(INT);
static void   setTxt(TCHAR** ptgt, TCHAR* src);
static TCHAR* get_libsDir(void);
static void   set_libsDir(TCHAR* s);
static TCHAR* get_spritesDir(void);
static void   set_spritesDir(TCHAR* s);
static TCHAR* get_extendedesDir(void);
static void   set_extendedesDir(TCHAR* s);
static TCHAR* get_clustersDir(void);
static void   set_clustersDir(TCHAR* s);
static INT    get_recentRomNums(void);
static INT    get_recentListNums(void);
static TCHAR* const * const get_RecentRoms(void);
static TCHAR* const * const get_RecentLists(void);
static void   replaceRecents(TCHAR*** ptgt, TCHAR** src);
static void   set_RecentRoms(TCHAR** ps);
static void   set_RecentLists(TCHAR** ps);
static DefineListItem* const * const get_definesLists(void);
static Define* MakeDefine(char* name, char* val, bool genCopy);
static List*  CreateDefinesList(void);
static bool   add_definesList(DefineListItem*);


/**
 * Main instance
 */
static IniMan* instance;

/**
 * Empty pointer
 */
static const TCHAR* const Empty = NULL;

/**
 * ini path
 */
TCHAR iniPath[MAX_PATH];
/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create IniMan object
 *
 * @return the pointer of object
 */
static IniMan* new_IniMan(void)
{
	IniMan_private* pri;

	/* make objects */
	instance = malloc(sizeof(IniMan));
	pri = malloc(sizeof(IniMan_private));

	/* check whether object creatin succeeded */
	assert(pri);
	assert(instance);

	/*--- set private member ---*/
	/*--- System ---*/
	pri->X = CW_USEDEFAULT;
	pri->Y = CW_USEDEFAULT;
	/*--- bool opt ---*/
	pri->isForce = FALSE;
	pri->isExtraBytes = FALSE;
	pri->isPixiCompatible = FALSE;
	pri->disableSscGen = FALSE;
	pri->isDebug = FALSE;
	pri->isDisableWarn = FALSE;
	/*--- defines ---*/
	pri->definesListNums = 5;
	pri->definesLists = NULL;
	/*--- dir opt ---*/
	pri->librariesDir = Str_copy(DefaultLibsDirName);
	pri->spritesDir = Str_copy(DefaultSprDirName);
	pri->extendedesDir = Str_copy(DefaultExtDirName);
	pri->clustersDir = Str_copy(DefaultClsDirName);
	/*--- recents ---*/
	pri->recentRomNums = pri->recentListNums = 10;
	pri->recentRomPaths = NULL;
	pri->recentListPaths = NULL;

	/*--- init methods ---*/
	instance->load = loadIni;
	instance->save = saveIni;
	instance->get_X = get_X;
	instance->set_X = set_X;
	instance->get_Y = get_Y;
	instance->set_Y = set_Y;
	instance->get_libsDir = get_libsDir;
	instance->set_libsDir = set_libsDir;
	instance->get_spritesDir = get_spritesDir;
	instance->set_spritesDir = set_spritesDir;
	instance->get_extendedesDir = get_extendedesDir;
	instance->set_extendedesDir = set_extendedesDir;
	instance->get_clustersDir = get_clustersDir;
	instance->set_clustersDir = set_clustersDir;
	instance->get_isForce = get_isForce;
	instance->set_isForce = set_isForce;
	instance->get_isExtraBytes = get_isExtraBytes;
	instance->set_isExtraBytes = set_isExtraBytes;
	instance->get_isPixiCompatible = get_isPixiCompatible;
	instance->set_isPixiCompatible = set_isPixiCompatible;
	instance->get_disableSscGen = get_disableSscGen;
	instance->set_disableSscGen = set_disableSscGen;
	instance->get_isDebug = get_isDebug;
	instance->set_isDebug = set_isDebug;
	instance->get_isDisableWarn = get_isDisableWarn;
	instance->set_isDisableWarn = set_isDisableWarn;
	instance->get_recentRomNums = get_recentRomNums;
	instance->get_recentListNums = get_recentListNums;
	instance->get_RecentRoms = get_RecentRoms;
	instance->get_RecentLists = get_RecentLists;
	instance->set_RecentRoms = set_RecentRoms;
	instance->set_RecentLists = set_RecentLists;
	instance->get_definesLists = get_definesLists;
	instance->add_definesList = add_definesList;

	/* init IniMan object */
	instance->pri = pri;
	return instance;
}

static void dList(void* p)
{
	DefineListItem* d = (DefineListItem*)p;

	if(NULL==d) return;
	free(d->name);
	delete_List(&d->defines);
	free(d);
}
static void cleanSub(void** tgt, void (*cleaner)(void*))
{
	uint i;
	assert(cleaner);
	if(NULL == tgt) return;

	for(i=0; NULL != tgt[i]; i++) cleaner(tgt[i]);
	free(tgt);
}
/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
static void delete_IniMan_members(IniMan* self)
{
	IniMan_private* pri;
	/* delete private members */
	pri = self->pri;
	cleanSub((void**)pri->definesLists, dList);
	cleanSub((void**)pri->recentRomPaths, free);
	cleanSub((void**)pri->recentListPaths, free);
	free(self->pri);
}

/**
 * @brief Delete IniMan object
 *
 * @param the pointer of object
 */
static void delete_IniMan(IniMan** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_IniMan_members(*self);
	free(*self);
	(*self) = NULL;
}

/*--------------- Instance getter ---------------*/
IniMan* IniMan_GetInstance(void)
{
	if(NULL == instance)
	{
#ifndef NDEBUG
		assert(new_IniMan());
#else
		new_IniMan();
#endif
		sprintf_s(iniPath, MAX_PATH, _T("%s%s"), Environment.ExeDir, IniName);
	}

	return instance;
}

/*--------------- Instance deleter ---------------*/
void IniMan_DiscardInstance(void)
{
	if(NULL != instance)
	{
		delete_IniMan(&instance);
	}
}



/*--------------- internal methods ---------------*/

static INT    get_X(void)			{ return instance->pri->X; }
static void   set_X(INT v)			{ instance->pri->X = v; }
static INT    get_Y(void)			{ return instance->pri->Y; }
static void   set_Y(INT v)			{ instance->pri->Y = v; }
static BOOL   get_isForce(void)			{ return instance->pri->isForce; }
static void   set_isForce(BOOL b)		{ instance->pri->isForce = b; }
static BOOL   get_isExtraBytes(void)		{ return instance->pri->isExtraBytes; }
static void   set_isExtraBytes(BOOL b)		{ instance->pri->isExtraBytes = b; }
static BOOL   get_isPixiCompatible(void)	{ return instance->pri->isPixiCompatible; }
static void   set_isPixiCompatible(BOOL b)	{ instance->pri->isPixiCompatible = b; }
static BOOL   get_disableSscGen(void)		{ return instance->pri->disableSscGen; }
static void   set_disableSscGen(BOOL b)		{ instance->pri->disableSscGen = b; }
static BOOL   get_isDebug(void)			{ return instance->pri->isDebug; }
static void   set_isDebug(BOOL b)		{ instance->pri->isDebug = b; }
static BOOL   get_isDisableWarn(void)		{ return instance->pri->isDisableWarn; }
static void   set_isDisableWarn(BOOL b)		{ instance->pri->isDisableWarn = b; }
static TCHAR* get_libsDir(void)			{ return instance->pri->librariesDir; }
static void   set_libsDir(TCHAR* s)		{ setTxt(&instance->pri->librariesDir,s); }
static TCHAR* get_spritesDir(void)		{ return instance->pri->spritesDir; }
static void   set_spritesDir(TCHAR* s)		{ setTxt(&instance->pri->spritesDir,s); }
static TCHAR* get_extendedesDir(void)		{ return instance->pri->extendedesDir; }
static void   set_extendedesDir(TCHAR* s)	{ setTxt(&instance->pri->extendedesDir,s); }
static TCHAR* get_clustersDir(void)		{ return instance->pri->clustersDir; }
static void   set_clustersDir(TCHAR* s)		{ setTxt(&instance->pri->clustersDir,s); }
static INT    get_recentRomNums(void)		{ return instance->pri->recentRomNums; }
static INT    get_recentListNums(void)		{ return instance->pri->recentListNums; }
static TCHAR* const * const get_RecentRoms(void) { return instance->pri->recentRomPaths; }
static TCHAR* const * const get_RecentLists(void) { return instance->pri->recentListPaths; }
static void   set_RecentRoms(TCHAR** ps)	{ replaceRecents(&instance->pri->recentRomPaths, ps); }
static void   set_RecentLists(TCHAR** ps)	{ replaceRecents(&instance->pri->recentListPaths, ps); }
static DefineListItem* const * const get_definesLists(void) { return instance->pri->definesLists; }

CIniMan cIniMan = {
	MakeDefine,
	CreateDefinesList
};

static void setTxt(TCHAR** ptgt, TCHAR* src)
{
	free(*ptgt);
	(*ptgt) = Str_copy(src);
}

static void replaceRecents(TCHAR*** ptgt, TCHAR** src)
{
	TCHAR** prevs;
	uint i;
	assert(ptgt);

	/* clean previous list */
	prevs = (*ptgt);
	if(NULL!=prevs)
	{
		for(i=0;NULL!=prevs[i];i++) free(prevs[i]);
		free(prevs);
	}

	/* replace pointer */
	(*ptgt) = src;
}

static bool GetPVStr(LPCTSTR sec, LPCTSTR key, TCHAR** pval, bool bKeep)
{
	TCHAR buf[512];
	assert(sec);
	assert(key);
	assert(pval);

	GetPrivateProfileString(sec, key, "", buf, 512, iniPath);
	switch(GetLastError())
	{
		case 0: /* No error */
			break;
		case 2: /* File not found */
			if(!bKeep)
			{
				free(*pval);
				(*pval) = NULL;
			}
			return true;
		default:
			return false;
	}

	if(0 == strlen(buf))
	{
		if(!bKeep)
		{
			free(*pval);
			(*pval) = NULL;
		}
		return true;
	}

	if(NULL != (*pval)) free(*pval);

	(*pval) = Str_copy(buf);
	return true;
}

static bool GetPVInt(LPCTSTR sec, LPCTSTR key, INT* pval)
{
	UINT v;
	assert(sec);
	assert(key);
	assert(pval);

	v = GetPrivateProfileInt(sec, key, (*pval), iniPath);
	switch(GetLastError())
	{
		case 0: /* No error */
			break;
		case 2: /* File not found */
			return true;
		default:
			return false;
	}

	(*pval) = (INT)v;
	return true;
}

static bool WritePVStr(LPCTSTR sec, LPCTSTR key, TCHAR* const * const pval)
{
	assert(sec);
	assert(pval);

	return WritePrivateProfileString(sec, key, (*pval), iniPath) ? true : false;
}
static bool WritePVInt(LPCTSTR sec, LPCTSTR key, INT* pval)
{
	TCHAR buf[64];
	TCHAR* p = buf;

	assert(sec);
	assert(pval);

	sprintf_s(buf, 64, _T("%d"), (*pval));
	return WritePVStr(sec, key, &p);
}


/*----- Ini confs -----*/
#define SecSystem		_T("System")
#define KeyX			_T("X")
#define KeyY			_T("Y")
#define KeyRecentRomNums	_T("RecentRomNums")
#define KeyRecentListNums	_T("RecentListNums")
#define KeyDefinesListNums	_T("DefinesListNums")

#define SecFlags		_T("Flags")
#define KeyIsForce		_T("IsForce")
#define KeyIsExtraBytes		_T("IsExtraBytes")
#define KeyIsPixiCompatible	_T("IsPixiCompatible")
#define KeyDisableSscGen	_T("DisableSscGen")
#define KeyIsDebug		_T("IsDebug")
#define KeyIsDisableWarn	_T("IsDisableWarn")

#define SecDirs			_T("Dirs")
#define KeyLibrariesDir		_T("LibrariesDir")
#define KeySpritesDir		_T("SpritesDir")
#define KeyExtendedesDir	_T("ExtendedesDir")
#define KeyClustersDir		_T("ClustersDir")

#define SecDefines		_T("Defines")
#define SecRecentRoms		_T("RecentRoms")
#define SecRecentLists		_T("RecentLists")

static bool loadRecent(LPCTSTR sec, TCHAR*** ppaths, const INT nums)
{
	uint i;
	TCHAR key[256];
	TCHAR** paths;

	assert(sec);
	assert(ppaths);

	paths = malloc(sizeof(TCHAR**) * (UINT)(nums+1));
	assert(paths);
	(*ppaths) = paths;
	for(i=0;i<nums+1;i++) paths[i] = NULL;

	for(i=0; i<nums; i++)
	{
		sprintf_s(key, 256, _T("path(%d)"), i);
		if(!GetPVStr(sec, key, &paths[i], false)) return false;
		if(NULL == paths[i]) break;
	}

	return true;
}

static void delDefs(void* d)
{
	Define* def = (Define*)d;

	if(NULL==def) return;
	free(def->name);
	free(def->val);
	free(def);
}
static List* CreateDefinesList(void)
{
	return new_List(NULL, delDefs);
}
static Define* MakeDefine(char* name, char* val, bool genCopy)
{
	Define* def;

	assert(name);
	assert(val);

	def = malloc(sizeof(Define));
	assert(def);

	if(genCopy)
	{
		def->name = Str_copy(name);
		def->val = Str_copy(val);
	}
	else
	{
		def->name = name;
		def->val = val;
	}

	return def;
}
static bool loadDefines(LPCTSTR sec, List* defs)
{
	TCHAR key[256];
	TCHAR* rdata = NULL;
	TCHAR* work;
	int ctr;
	size_t i;
	size_t len;
	Define* def;

	for(ctr=0;;ctr++)
	{
		sprintf_s(key, 256, _T("define(%d)"), ctr);
		if(!GetPVStr(sec, key, &rdata, false))
		{
			free(rdata);
			return false;
		}
		if(NULL == rdata) break;

		work = Str_copy(rdata);
		i=0;
		len = strlen(work);

		SkipUntilChar(work, &i, ':', len);
		if(len <= i)
		{
			/* name not found, skip */
			free(work);
			continue;
		}
		work[i++] = '\0';
		if(len <= i)
		{
			/* value not found */
			def = MakeDefine(work,_T("1"),true);
			defs->push(defs, def);
			free(work);
			continue;
		}
		def = MakeDefine(work,&work[i],true);
		defs->push(defs, def);
		free(work);
	}
	return true;
}
static bool loadDefineSections(void)
{
	TCHAR sec[256];
	uint i;
	TCHAR* name = NULL;

	instance->pri->definesLists = malloc(sizeof(DefineListItem*) * (uint)(instance->pri->definesListNums+1));
	assert(instance->pri->definesLists);
	for(i=0; i<instance->pri->definesListNums+1; i++)
	{
		instance->pri->definesLists[i] = NULL;
	}
	for(i=0; i<instance->pri->definesListNums; i++)
	{
		sprintf_s(sec, 256, SecDefines _T("(%d)"), i);
		if(!GetPVStr(sec, _T("Name"), &name, false)) return false;
		if(NULL == name) break;
		instance->pri->definesLists[i] = malloc(sizeof(DefineListItem));
		assert(instance->pri->definesLists[i]);
		instance->pri->definesLists[i]->name = name;
		name = NULL;
		instance->pri->definesLists[i]->defines = new_List(NULL, delDefs);
		assert(instance->pri->definesLists[i]);
		if(!loadDefines(sec, instance->pri->definesLists[i]->defines)) return false;
	}

	return true;
}
static bool loadIni(void)
{
	/*** Load "System" section ***/
	if(!GetPVInt(SecSystem, KeyX, &instance->pri->X)) return false;
	if(!GetPVInt(SecSystem, KeyY, &instance->pri->Y)) return false;
	if(!GetPVInt(SecSystem, KeyRecentRomNums, &instance->pri->recentRomNums)) return false;
	if(!GetPVInt(SecSystem, KeyRecentListNums, &instance->pri->recentListNums)) return false;
	if(!GetPVInt(SecSystem, KeyDefinesListNums, &instance->pri->definesListNums)) return false;

	/*** Load "Flags" section ***/
	if(!GetPVInt(SecFlags, KeyIsForce, &instance->pri->isForce)) return false;
	if(!GetPVInt(SecFlags, KeyIsExtraBytes, &instance->pri->isExtraBytes)) return false;
	if(!GetPVInt(SecFlags, KeyIsPixiCompatible, &instance->pri->isPixiCompatible)) return false;
	if(!GetPVInt(SecFlags, KeyDisableSscGen, &instance->pri->disableSscGen)) return false;
	if(!GetPVInt(SecFlags, KeyIsDebug, &instance->pri->isDebug)) return false;
	if(!GetPVInt(SecFlags, KeyIsDisableWarn, &instance->pri->isDisableWarn)) return false;

	/*** Load "Dirs" section ***/
	if(!GetPVStr(SecDirs, KeyLibrariesDir, &instance->pri->librariesDir, true)) return false;
	if(!GetPVStr(SecDirs, KeySpritesDir, &instance->pri->spritesDir, true)) return false;
	if(!GetPVStr(SecDirs, KeyExtendedesDir, &instance->pri->extendedesDir, true)) return false;
	if(!GetPVStr(SecDirs, KeyClustersDir, &instance->pri->clustersDir, true)) return false;

	/*** Load "Defines(XX)" sections ***/
	if(!loadDefineSections()) return false;

	/*** Load "RecentRoms" section ***/
	if(!loadRecent(SecRecentRoms, &instance->pri->recentRomPaths, instance->pri->recentRomNums)) return false;

	/*** Load "RecentLists" section ***/
	if(!loadRecent(SecRecentLists, &instance->pri->recentListPaths, instance->pri->recentListNums)) return false;

	return true;
}

static bool saveRecent(LPCTSTR sec, TCHAR*** ppaths, const INT nums)
{
	uint i;
	TCHAR key[256];
	TCHAR** paths = (*ppaths);

	assert(sec);
	assert(ppaths);

	if(NULL == paths) return true; /* Nothing to do */

	/* write config */
	for(i=0; NULL!=paths[i]; i++)
	{
		sprintf_s(key, 256, _T("path(%d)"), i);
		if(!WritePVStr(sec, key, &paths[i])) return false;
	}

	/* clean-up key */
	for(; i<nums; i++)
	{
		sprintf_s(key, 256, _T("path(%d)"), i);
		if(!WritePVStr(sec, key, (TCHAR**)&Empty)) return false;
	}

	return true;
}

static bool saveDefines(LPCTSTR sec, DefineListItem* defs)
{
	TCHAR key[256];
	TCHAR val[256];
	TCHAR* p = val;
	Iterator* it;
	Define* def;
	uint i;

	/* clean-up */
	if(!WritePVStr(sec, NULL, (TCHAR**)&Empty)) return false;

	/* write define name */
	if(!WritePVStr(sec, _T("Name"), &defs->name)) return false;

	/* write defines */
	i = 0;
	for(it=defs->defines->begin(defs->defines);NULL!=it;it=it->next(it))
	{
		def = (Define*)it->data(it);
		sprintf_s(key, 256, _T("define(%d)"), i++);
		sprintf_s(val, 256, _T("%s:%s"), def->name, def->val);
		if(!WritePVStr(sec, key, &p)) return false;
	}

	return true;
}
static bool saveDefineSections(void)
{
	TCHAR sec[256];
	uint i;

	if(NULL == instance->pri->definesLists) return true; /* Nothing to do */

	/* write config */
	for(i=0; NULL != instance->pri->definesLists[i]; i++)
	{
		sprintf_s(sec, 256, SecDefines _T("(%d)"), i);
		/* write */
		if(!saveDefines(sec, instance->pri->definesLists[i])) return false;
	}
	/* clean-up section */
	for(;i<instance->pri->definesListNums; i++)
	{
		sprintf_s(sec, 256, SecDefines _T("(%d)"), i);
		if(!WritePVStr(sec, NULL, (TCHAR**)&Empty)) return false;
	}

	return true;
}

static bool saveIni(void)
{
	/*** Save "System" section ***/
	if(!WritePVInt(SecSystem, KeyX, &instance->pri->X)) return false;
	if(!WritePVInt(SecSystem, KeyY, &instance->pri->Y)) return false;
	if(!WritePVInt(SecSystem, KeyRecentRomNums, &instance->pri->recentRomNums)) return false;
	if(!WritePVInt(SecSystem, KeyRecentListNums, &instance->pri->recentListNums)) return false;
	if(!WritePVInt(SecSystem, KeyDefinesListNums, &instance->pri->definesListNums)) return false;

	/*** Save "Flags" section ***/
	if(!WritePVInt(SecFlags, KeyIsForce, &instance->pri->isForce)) return false;
	if(!WritePVInt(SecFlags, KeyIsExtraBytes, &instance->pri->isExtraBytes)) return false;
	if(!WritePVInt(SecFlags, KeyIsPixiCompatible, &instance->pri->isPixiCompatible)) return false;
	if(!WritePVInt(SecFlags, KeyDisableSscGen, &instance->pri->disableSscGen)) return false;
	if(!WritePVInt(SecFlags, KeyIsDebug, &instance->pri->isDebug)) return false;
	if(!WritePVInt(SecFlags, KeyIsDisableWarn, &instance->pri->isDisableWarn)) return false;

	/*** Save "Dirs" section ***/
	if(!WritePVStr(SecDirs, KeyLibrariesDir, &instance->pri->librariesDir)) return false;
	if(!WritePVStr(SecDirs, KeySpritesDir, &instance->pri->spritesDir)) return false;
	if(!WritePVStr(SecDirs, KeyExtendedesDir, &instance->pri->extendedesDir)) return false;
	if(!WritePVStr(SecDirs, KeyClustersDir, &instance->pri->clustersDir)) return false;

	/*** Save "Defines(XX)" sections ***/
	if(!saveDefineSections()) return false;

	/*** Save "RecentRoms" section ***/
	if(!saveRecent(SecRecentRoms, &instance->pri->recentRomPaths, instance->pri->recentRomNums)) return false;

	/*** Save "RecentLists" section ***/
	if(!saveRecent(SecRecentLists, &instance->pri->recentListPaths, instance->pri->recentListNums)) return false;

	return true;
}

static bool add_definesList(DefineListItem* dli)
{
	DefineListItem* old = NULL;
	int i,j;

	if(NULL==dli) return false;
	if(NULL==dli->name) return false;
	if(NULL==dli->defines) return false;

	if(NULL == instance->pri->definesLists)
	{
		int x;
		DefineListItem** tmp;
		tmp = malloc((uint)(instance->pri->definesListNums+1)*sizeof(DefineListItem*));
		if(NULL==tmp) return false;
		for(x=0;x<(instance->pri->definesListNums+1);x++) tmp[x]=NULL;
		instance->pri->definesLists = tmp;
	}

	/* count contents */
	for(i=0; i<instance->pri->definesListNums; i++)
	{
		if(NULL==instance->pri->definesLists[i]) break;
		if(0 == strcmp(dli->name, instance->pri->definesLists[i]->name))
		{
			old = instance->pri->definesLists[i];
		}
	}

	if(i==instance->pri->definesListNums && NULL==old)
	{
		dList(instance->pri->definesLists[--i]);
	}
	j = i--;
	if(old!=NULL) i=j;

	for(;0<=i; i--,j--)
	{
		if(instance->pri->definesLists[i] == old)
		{
			i--;
			if(0>i) break;
		}
		instance->pri->definesLists[j] = instance->pri->definesLists[i];
	}
	dList(old);
	instance->pri->definesLists[0] = dli;

	return true;
}
