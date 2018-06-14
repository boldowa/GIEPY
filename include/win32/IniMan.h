#pragma once
/**
 * @file IniMan.h
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _DefineListItem{
	TCHAR* name;
	List* defines;
} DefineListItem;

/**
 * public accessor
 */
typedef struct _IniMan IniMan;
typedef struct _IniMan_protected IniMan_protected;
typedef struct _IniMan_private IniMan_private;
struct _IniMan {
	/* member method */
	bool (*load)(void);
	bool (*save)(void);
	BOOL (*get_isForce)(void);
	void (*set_isForce)(BOOL);
	BOOL (*get_isExtraBytes)(void);
	void (*set_isExtraBytes)(BOOL);
	BOOL (*get_isPixiCompatible)(void);
	void (*set_isPixiCompatible)(BOOL);
	BOOL (*get_disableSscGen)(void);
	void (*set_disableSscGen)(BOOL);
	BOOL (*get_isDebug)(void);
	void (*set_isDebug)(BOOL);
	BOOL (*get_isDisableWarn)(void);
	void (*set_isDisableWarn)(BOOL);
	INT (*get_X)(void);
	void (*set_X)(INT);
	INT (*get_Y)(void);
	void (*set_Y)(INT);
	TCHAR* (*get_libsDir)(void);
	void   (*set_libsDir)(TCHAR* s);
	TCHAR* (*get_spritesDir)(void);
	void   (*set_spritesDir)(TCHAR* s);
	TCHAR* (*get_extendedesDir)(void);
	void   (*set_extendedesDir)(TCHAR* s);
	TCHAR* (*get_clustersDir)(void);
	void   (*set_clustersDir)(TCHAR* s);
	INT    (*get_recentRomNums)(void);
	INT    (*get_recentListNums)(void);
	TCHAR* const * const (*get_RecentRoms)(void);
	TCHAR* const * const (*get_RecentLists)(void);
	void   (*set_RecentRoms)(TCHAR** ps);
	void   (*set_RecentLists)(TCHAR** ps);
	DefineListItem* const * const (*get_definesLists)(void);
	bool   (*add_definesList)(DefineListItem*);

	/* private members */
	IniMan_private* pri;
};

typedef struct _CIniMan {
	Define* (*MakeDefine)(char* name, char* val, bool genCopy);
	List* (*CreateDefinesList)(void);
}CIniMan;
extern CIniMan cIniMan;

/**
 * Constructor
 */
/*IniMan* new_IniMan(void);*/

/**
 * Destractor
 */
/*void delete_IniMan(IniMan**);*/

/**
 * Instance getter
 */
IniMan* IniMan_GetInstance(void);

/**
 * Instance deleter
 */
void IniMan_DiscardInstance(void);

#ifdef __cplusplus
} /*extern "C" */
#endif
