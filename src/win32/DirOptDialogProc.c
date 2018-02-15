/**
 * @file DirOptDialogProc.c
 */
#include "common/types.h"
#include <windows.h>
#include <tchar.h>
#include "common/List.h"
#include "common/defines.h"
#include "common/strres.h"
#include "common/GetFmtStr.h"
#include "common/Observer.h"
#include "win32/IniMan.h"
#include "win32/PieeDialog.h"
#include "win32/DirOptDialogProc.h"

#define ID_EDIT_LIBRARIES	100
#define ID_EDIT_SPRITES		101
#define ID_EDIT_EXTENDEDES	102
#define ID_EDIT_CLUSTERS	103

extern HINSTANCE mainInstance;

typedef enum {
	CtlInitType_Static,
	CtlInitType_Edit,
	CtlInitType_OKBtn
}CtlInitType;

static bool IsSpace(TCHAR c)
{
	static const TCHAR sp[] = {
		_T(' '), _T('\t'), _T('\r'), _T('\n')
	};
	static const size_t spLen = sizeof(sp)/sizeof(TCHAR);
	size_t i;

	for(i=0;i<spLen;i++)
	{
		if(sp[i] == c) return true;
	}
	return false;
}

static void RemoveSpaces(TCHAR* str, size_t* plen)
{
	size_t i;

	i=(*plen);
	for(;0<i;i--)
	{
		if(!IsSpace(str[i])) break;
		str[i] = _T('\0');
	}
	(*plen) = i;
}

static void OnOK(HWND hWnd, WORD wNotify, HWND hControl)
{
	TCHAR buf[MAX_PATH];
	HWND hCtl;
	IniMan* ini = IniMan_GetInstance();
	uint i;
	size_t len;
	const struct {
		INT idEdit;
		UINT idStr;
		void (*proc)(TCHAR*);
	} ctlSaveData[] = {
		{ ID_EDIT_LIBRARIES, GSID_DIROPTDLG_LIBRARIES, ini->set_libsDir },
		{ ID_EDIT_SPRITES, GSID_DIROPTDLG_SPRITES, ini->set_spritesDir },
		{ ID_EDIT_EXTENDEDES, GSID_DIROPTDLG_EXTENDEDES, ini->set_extendedesDir },
		{ ID_EDIT_CLUSTERS, GSID_DIROPTDLG_CLUSTERS, ini->set_clustersDir },
		{ 0, 0, NULL }
	};

	/* Valid check */
	for(i=0;NULL!=ctlSaveData[i].proc;i++)
	{
		hCtl = GetDlgItem(hWnd, ctlSaveData[i].idEdit);
		GetWindowText(hCtl, buf, sizeof(TCHAR)*MAX_PATH);
		len = _tcslen(buf);
		RemoveSpaces(buf, &len);
		if(0 == len)
		{
			puterrimm(0, GSID_INPUTERR_EMPTY, GetFmtStr(ctlSaveData[i].idStr));
			return;
		}
	}

	/* Save values */
	for(i=0;NULL!=ctlSaveData[i].proc;i++)
	{
		hCtl = GetDlgItem(hWnd, ctlSaveData[i].idEdit);
		GetWindowText(hCtl, buf, sizeof(TCHAR)*MAX_PATH);
		len = _tcslen(buf);
		RemoveSpaces(buf, &len);
		ctlSaveData[i].proc(buf);
	}
	
	/* Close dialog */
	PostMessage(hWnd, WM_CLOSE, 0,0);
}

static HWND PutControl(HWND hWnd, HINSTANCE hInstance, CtlInitType type, INT y, HMENU id, LPCTSTR txt)
{
	static const INT height = 20;
	INT width = 260;
	INT x = 10;
	TCHAR* pszWindowClass;
	DWORD style;
	DWORD exStyle;
	HWND hCtl;
	switch(type)
	{
		case CtlInitType_Static:
			pszWindowClass = _T("STATIC");
			style = SS_SIMPLE;
			exStyle = 0;
			break;
		case CtlInitType_Edit:
			pszWindowClass = _T("EDIT");
			style = WS_TABSTOP;
			exStyle = WS_EX_CLIENTEDGE;
			break;
		case CtlInitType_OKBtn:
			pszWindowClass = _T("BUTTON");
			style = BS_PUSHBUTTON;
			exStyle = 0;
			x = 240;
			width = 40;
			break;
	}

	hCtl = CreateWindowEx(
			exStyle, pszWindowClass, txt,
			WS_CHILD|WS_VISIBLE|style,
			x, y,
			width, height,
			hWnd, id, hInstance, NULL);
	SendMessage(hCtl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE,0));
	return hCtl;
}

static void OnInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	static const INT width = 300;
	static const INT height = 250;
	static const INT labHeight = 14;
	HWND hCtl;
	uint i;
	IniMan* ini = IniMan_GetInstance();
	const struct {
		INT y;
		UINT idStr;
		UINT idEdit;
		TCHAR* (*proc)();
	} ctlInitData[] = {
		{  10, GSID_DIROPTDLG_LIBRARIES, ID_EDIT_LIBRARIES, ini->get_libsDir },
		{  50, GSID_DIROPTDLG_SPRITES, ID_EDIT_SPRITES, ini->get_spritesDir },
		{  90, GSID_DIROPTDLG_EXTENDEDES, ID_EDIT_EXTENDEDES, ini->get_extendedesDir },
		{ 130, GSID_DIROPTDLG_CLUSTERS, ID_EDIT_CLUSTERS, ini->get_clustersDir },
		{   0, 0, 0, NULL }
	};

	SetPieeDlgCaption(hWnd,GetFmtStr(GSID_DIROPTDLG));
	PieeCenterDialog(hWnd, width, height);

	for(i=0;NULL!=ctlInitData[i].proc;i++)
	{
		       PutControl(hWnd, mainInstance, CtlInitType_Static,  ctlInitData[i].y, 0, GetFmtStr(ctlInitData[i].idStr));
		hCtl = PutControl(hWnd, mainInstance, CtlInitType_Edit,    ctlInitData[i].y+labHeight, (HMENU)ctlInitData[i].idEdit, _T(""));
		SetWindowText(hCtl,ctlInitData[i].proc());
	}
}

static LRESULT CommandHandler(HWND hWnd, WORD wId, WORD wNotify, HWND hControl, bool *bHandled)
{
	switch(wId)
	{
		case IDOK:
			OnOK(hWnd, wNotify, hControl);
			return 0;

		case IDCANCEL:
			PostMessage(hWnd, WM_CLOSE, 0,0);
			return 0;
	}

	(*bHandled) = false;
	return 0;
}

static void ReposOKCancelButton(HWND hDlg)
{
	HWND hOK, hCancel;
	RECT rDlg, rOK, rCancel;

	hOK = GetDlgItem(hDlg, IDOK);
	hCancel = GetDlgItem(hDlg, IDCANCEL);

	GetClientRect(hDlg,&rDlg);
	GetClientRect(hOK,&rOK);
	GetClientRect(hCancel,&rCancel);

	SetWindowPos(hCancel, HWND_TOP,
			rDlg.right  - (rCancel.right-rCancel.left) - 10,
			rDlg.bottom - (rCancel.bottom-rCancel.top) - 10,
			0,0,
			SWP_NOSIZE|SWP_NOZORDER);

	SetWindowPos(hOK, HWND_TOP,
			rDlg.right  - (rOK.right-rOK.left) - rCancel.right  - 15,
			rDlg.bottom - (rOK.bottom-rOK.top) - 10,
			0,0,
			SWP_NOSIZE|SWP_NOZORDER);
}

INT_PTR CALLBACK DirOptDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT r;
	bool bHandled = true;

	switch(msg)
	{
		case WM_INITDIALOG:
			OnInit(hWnd, wParam, lParam);
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;

		case WM_SIZE:
			ReposOKCancelButton(hWnd);
			return TRUE;

		case WM_COMMAND:
			r = CommandHandler(hWnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam, &bHandled);
			if(bHandled) return r;
			break;
	}
	return FALSE;
}
