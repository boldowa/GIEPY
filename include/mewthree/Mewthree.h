#pragma once
/**
 * @file Mewthree.h
 */

/**
 * public accessor
 */
typedef struct _Mewthree Mewthree;
typedef struct _Mewthree_protected Mewthree_protected;
typedef struct _Mewthree_private Mewthree_private;
struct _Mewthree {
	/*---------- member method ----------*/
	/* observer */
	void (*RegisterObserver)(Mewthree*, Observers);

	/* env */
	void (*SetSystemPath)(Mewthree*, const char*, const char*);
	void (*ReleaseEnvironment)(Mewthree*);

	/* rom */
	bool (*OpenRomFile)(Mewthree*, const char*);
	bool (*WriteRomFile)(Mewthree*);
	void (*CloseRomFile)(Mewthree*);
	bool (*UninstallAll)(Mewthree*);
	bool (*UninstallMain)(Mewthree*);
	bool (*Clean)(Mewthree*);

	/* asm */
	AsmType (*ChkAsmType)(Mewthree*, const char*);

	/* list */
	bool (*ParseList)(Mewthree*, const char* listName);
	List* (*GetInsertList)(Mewthree*);		/* for GUI app */

	/* insert */
	void (*InitializeWorkspace)(Mewthree*, List*);
	bool (*CollectLibraries)(Mewthree*, const char*);
	bool (*InitializeRom)(Mewthree*, bool, bool, bool);
	bool (*AssembleAll)(Mewthree*, srcdirs* dir);
	bool (*InsertSprite)(Mewthree*);
	bool (*InsertExtended)(Mewthree*);
	bool (*InsertCluster)(Mewthree*);
	bool (*InsertOverworld)(Mewthree*);
	bool (*CheckDataLeak)(Mewthree*);
	void (*CleanWorkspace)(Mewthree*);
	void (*RoughTimer)(Mewthree* m, uint msecs, uint interval);

	/* cfg */
	CfgData* (*ParseCfg)(Mewthree*, const char* path);
	CfgData* (*ParseJson)(Mewthree*, const char* path);
	void (*DiscardCfgData)(Mewthree* mi, CfgData** d);

	/*---------- main instance ----------*/
	Mewthree_protected* pro;
};

/**
 * Constructor
 */
Mewthree* new_Mewthree(void);

/**
 * Destractor
 */
void delete_Mewthree(Mewthree**);

