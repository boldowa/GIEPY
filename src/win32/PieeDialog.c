/**
 * @file PieeDialog.c
 */
#include <bolib.h>
#include <windows.h>
#include <tchar.h>
#include "common/strres.h"
#include "common/GetFmtStr.h"
#include "win32/menures.h"
#include "win32/DirOptDialogProc.h"
#include "win32/InfoDialogProc.h"
#include "win32/PieeDialog.h"

static DLGPROC GetPieeDlgProc(UINT uId)
{
	switch(uId)
	{
		case ID_DLG_DIROPTION:	return DirOptDialogProc;
		case ID_DLG_INFO:	return InfoDialogProc;
	}
	return NULL;
}

LRESULT PieeDialog(HWND hParent, WORD wId, LPVOID lpData)
{
	DLGPROC lpfnDlgProc;

	if(NULL == hParent) return -1;

	lpfnDlgProc = GetPieeDlgProc(wId);
	if(NULL==lpfnDlgProc) return -1;

	return DialogBoxParam(
			GetModuleHandle(NULL),
			_T("EmptyDlg"),
			hParent,
			lpfnDlgProc,
			(LPARAM)lpData);
}

BOOL PieeCenterDialog(HWND hDlg, INT width, INT height)
{
	RECT rParent;
	INT pCx, pCy;

	if(0 == GetWindowRect(GetDlgParent(hDlg), &rParent)) return FALSE;
	pCx = rParent.right - rParent.left;
	pCy = rParent.bottom - rParent.top;

	return MoveWindow(hDlg,
			rParent.left + ((pCx -  width) / 2),
			rParent.top  + ((pCy - height) / 2),
			width, height,
			FALSE);
}

void SetPieeDlgCaption(HWND hDlg, LPCTSTR caption)
{
	HWND hCtl;

	/* set OK button font and caption */
	hCtl = GetDlgItem(hDlg,IDOK);
	SendMessage(hCtl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE,0));
	SetWindowText(hCtl, GetFmtStr(GSID_BUTTON_OK));

	/* set Cancel button font and caption */
	hCtl = GetDlgItem(hDlg,IDCANCEL);
	SendMessage(hCtl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE,0));
	SetWindowText(hCtl, GetFmtStr(GSID_BUTTON_CANCEL));

	/* set dialog caption */
	SetWindowText(hDlg, caption);
}

