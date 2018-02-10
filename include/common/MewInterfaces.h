#pragma once
/**
 * @file MewInterfaces.h
 */

#include "common/AsmType.h"
#include "common/InsertList.h"
#include "common/CfgData.h"

typedef struct _MewInterface MewInterface;
struct _MewInterface {
	/*---------- member method i/f ----------*/
	/* observer */
	void (*RegisterObserver)(MewInterface*, Observers obs);

	/* rom */
	bool (*OpenRomFile)(MewInterface*, const char* path);
	bool (*WriteRomFile)(MewInterface*);
	void (*CloseRomFile)(MewInterface*);
	bool (*UninstallAll)(MewInterface*);
	bool (*UninstallMain)(MewInterface*);
	bool (*CleanData)(MewInterface*);

	/* asm */
	AsmType (*ChkAsmType)(MewInterface*, const char* path);

	/* env */
	void (*SetSystemPath)(MewInterface*, const char*, const char*);
	void (*ReleaseEnvironment)(MewInterface*);

	/* list */
	bool (*ParseList)(MewInterface*, const char* listName);
	List* (*GetInsertList)(MewInterface*);

	/* insert */
	void (*InitializeWorkspace)(MewInterface*, List*);
	bool (*CollectLibraries)(MewInterface*, const char*);
	bool (*InitializeRom)(MewInterface*, bool, bool, bool);
	bool (*AssembleAll)(MewInterface*, srcdirs* dir);
	bool (*InsertSprite)(MewInterface*);
	bool (*InsertExtended)(MewInterface*);
	bool (*InsertCluster)(MewInterface*);
	bool (*InsertOverworld)(MewInterface*);
	bool (*CheckDataLeak)(MewInterface*);
	void (*CleanWorkspace)(MewInterface*);
	void (*RoughTimer)(MewInterface* m, uint msecs, uint interval);

	/* cfg */
	CfgData* (*ParseCfg)(MewInterface*, const char* path);
	CfgData* (*ParseJson)(MewInterface*, const char* path);
	void (*DiscardCfgData)(MewInterface*, CfgData**);

	/* mewthree main instance. */
	/* (dll private instance)  */
	void* instance;
};

