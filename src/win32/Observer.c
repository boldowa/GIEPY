/**
 * @file Observer.c
 */

#include <bolib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "common/Observer.h"
#include "dll/Mewthree.def"
#include "common/GetFmtStr.h"
#include "win32/menures.h"
#include "win32/MainWndProc.h"
#include "win32/InfoDialogProc.h"
#include "win32/PieeDialog.h"
#include "cui/Console.h"

extern bool vdebug;
extern bool disableWarn;
ProgressMode pMode = ProgressMode_Normal;

static List* dbgList;
static List* warnList;
static List* errList;
static List* fatalList;
HWND hMainWnd = NULL;

bool InitObserver(HWND hWnd)
{
	dbgList = new_List(NULL,free);
	warnList = new_List(NULL,free);
	errList = new_List(NULL,free);
	fatalList = new_List(NULL,free);
	if(!dbgList||!warnList||!errList||!fatalList)
	{
		UninitObserver();
		return false;
	}
	hMainWnd = hWnd;
	return true;
}

void UninitObserver(void)
{
	delete_List(&dbgList);
	delete_List(&warnList);
	delete_List(&errList);
	delete_List(&fatalList);
}

typedef enum {
	ET_Debug = 0,
	ET_Warning,
	ET_Error,
	ET_Fatal,
	/*================*/
	ET_Term
} ErrorType;
void PieeShowError(void)
{
	static const struct {
		List** plist;
		ErrorType type;
		UINT captionId;
		UINT descId;
		UINT tabnameId;
		IconType icon;
	} PSEObj[] = {
		{ &dbgList, ET_Debug, GSID_INFDLG_DEBUG, GSID_INFDLG_DEBUG, GSID_INFDLG_TAB_DEBUG, IconType_Info },
		{ &warnList, ET_Warning, GSID_INFDLG_WARNING, GSID_INFDLG_WARNING_DESC, GSID_INFDLG_TAB_WARNING, IconType_Warning },
		{ &errList, ET_Error, GSID_INFDLG_ERROR, GSID_INFDLG_ERROR_DESC, GSID_INFDLG_TAB_ERROR, IconType_Error },
		{ &fatalList, ET_Fatal, GSID_INFDLG_ERROR, GSID_INFDLG_ERROR_DESC, GSID_INFDLG_TAB_FATAL, IconType_Error  },
		{ NULL, ET_Term, 0,0,0 }
	};
	uint i;
	List* list;
	List** plist;
	TabData tabs[ET_Term];
	InfDlgInitData dlgdata = {0};
	UINT idCap = 0;
	UINT idDesc = 0;

	dlgdata.tabs = tabs;
	for(plist=PSEObj[i=0].plist;NULL!=plist;plist=PSEObj[++i].plist)
	{
		list = (*plist);
		if(NULL==list) continue;
		if(0 != list->length(list))
		{
			dlgdata.tabs[dlgdata.nTabs].name = GetFmtStr(PSEObj[i].tabnameId);
			dlgdata.tabs[dlgdata.nTabs].msgList = list;
			dlgdata.icon = PSEObj[i].icon;
			dlgdata.nTabs++;
			idCap = PSEObj[i].captionId;
			idDesc = PSEObj[i].descId;
		}
	}
	if(0==dlgdata.nTabs) return;

	dlgdata.caption = GetFmtStr(idCap);
	dlgdata.description = GetFmtStr(idDesc);

	PieeDialog(hMainWnd, ID_DLG_INFO, &dlgdata);
}

static void PushStrCommon(List* list, uint id, va_list vl)
{
	TCHAR buf[2048];
	TCHAR* msg;
	size_t len;

	vsprintf(buf, GetFmtStr(id), vl);
	len = _tcslen(buf)+1;
	msg = calloc(len, sizeof(TCHAR));
	_tcscpy_s(msg, len, buf);
	list->push(list,msg);
}

static void SetStsTxt(uint id, va_list vl)
{
	HWND hSts;
	TCHAR buf[2048];

	hSts = GetDlgItem(hMainWnd, ID_STSBAR);
	if(!hSts) return;
	vsprintf(buf, GetFmtStr(id),vl);
	SendMessage(hSts, SB_SETTEXT, (WPARAM)0, (LPARAM)buf);
}

uint putdbg(uint flag, uint id, ...)
{
	va_list vl;

	if(vdebug)
	{
		va_start(vl, id);
		PushStrCommon(dbgList,id,vl);
		va_end(vl);
	}

	return MEW_OBS_OK;
}

uint putinf(uint flag, uint id, ...)
{
	va_list vl;

	va_start(vl, id);
	SetStsTxt(id,vl);
	if(vdebug) PushStrCommon(dbgList,id,vl);
	va_end(vl);

	return MEW_OBS_OK;
}

uint putques(uint flag, uint id, ...)
{
	va_list vl;
	char buf[2048];

	va_start(vl, id);
	vsprintf(buf, GetFmtStr(id), vl);
	if(IDYES == MessageBox(hMainWnd, buf, GetFmtStr(GSID_INQUIRY), MB_ICONQUESTION|MB_YESNO))
		return MEW_OBS_YES;
	
	return MEW_OBS_NO;
}

uint putwar(uint flag, uint id, ...)
{
	va_list vl;

	if(!disableWarn)
	{
		va_start(vl, id);
		PushStrCommon(warnList,id,vl);
		if(vdebug) PushStrCommon(dbgList,id,vl);
		va_end(vl);
	}

	return MEW_OBS_OK;
}

uint puterr(uint flag, uint id, ...)
{
	va_list vl;

	va_start(vl, id);
	PushStrCommon(errList,id,vl);
	if(vdebug) PushStrCommon(dbgList,id,vl);
	va_end(vl);

	return MEW_OBS_OK;
}

uint putfatal(uint flag, uint id, ...)
{
	va_list vl;

	va_start(vl, id);
	PushStrCommon(fatalList,id,vl);
	if(vdebug) PushStrCommon(dbgList,id,vl);
	va_end(vl);

	return MEW_OBS_OK;
}

uint puterrimm(uint flag, uint id, ...)
{
	va_list vl;
	TCHAR buf[2048];

	va_start(vl, id);

	vsprintf(buf, GetFmtStr(id), vl);
	MessageBox(hMainWnd, buf, GetFmtStr(GSID_ERROR), MB_ICONHAND);

	va_end(vl);

	return MEW_OBS_OK;
}

void progress(uint cur, uint all)
{
	SendNotifyMessage(hMainWnd, WM_PIEE_SETPROGRESS, (WPARAM)cur, (LPARAM)all);
}
