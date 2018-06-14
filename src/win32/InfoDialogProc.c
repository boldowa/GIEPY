/**
 * @file InfoDialogProc.c
 */
#include <bolib.h>
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "common/strres.h"
#include "common/GetFmtStr.h"
#include "win32/icon.h"
#include "win32/PieeDialog.h"
#include "win32/InfoDialogProc.h"

extern HINSTANCE mainInstance;

static TCHAR* con(TCHAR* const s1, const TCHAR* const s2)
{
	TCHAR* con;
	TCHAR* top;
	size_t len;
	size_t s1len;

	if(NULL == s1 || NULL == s2) return NULL;

	/* Memory allocation */
	len = _tcslen(s1) + _tcslen(s2);
	con = calloc(len+3, sizeof(TCHAR));
	top = con;
	
	/* Connect string */
	_tcscpy_s(con, len+2, s1);
	s1len = _tcslen(s1);
	con += s1len;
	len -= s1len;
	_tcscpy_s(con, len+2, s2);
	con[len+0] = _T('\r');
	con[len+1] = _T('\n');
	con[len+2] = _T('\0');

	return top;
}

static TCHAR* ConnectMessages(List* msgList)
{
	TCHAR* conmsg;
	TCHAR* txt;
	TCHAR* tmp;
	if(NULL==msgList) return NULL;
	if(0 == msgList->length(msgList)) return NULL;

	conmsg=calloc(1, sizeof(TCHAR));
	txt = msgList->popFront(msgList);
	while(NULL!=txt)
	{
		tmp = con(conmsg,txt);
		if(tmp != NULL)
		{
			free(conmsg);
			conmsg = tmp;
		}
		free(txt);
		txt = msgList->popFront(msgList);
	}
	return conmsg;
}

static void CreateTabs(HWND hWnd, HWND hTv, InfDlgInitData* data)
{
	HWND hTab;
	TC_ITEM tci;
	RECT rTab;
	uint i;
	TabData* tab;
	TCHAR* txt;

	tci.mask = TCIF_TEXT|TCIF_PARAM;
	if(0!=data->nTabs)
	{
		for(tab=&data->tabs[i=0]; i<data->nTabs; tab=&data->tabs[++i])
		{
			/* create edit control */
			hTab = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
					WS_CHILD|WS_VSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_READONLY,
					0, 0, 0, 0,
					hTv, NULL, mainInstance,
					NULL);
			SendMessage(hTab, WM_SETFONT, (WPARAM)GetStockObject(ANSI_FIXED_FONT), MAKELPARAM(TRUE,0));

			/* insert tab */
			tci.pszText = tab->name;
			tci.lParam = (LPARAM)hTab;
			SendMessage(hTv, TCM_INSERTITEM, (WPARAM)0, (LPARAM)&tci);

			/* resize edit control */
			GetClientRect(hTv,&rTab);
			SendMessage(hTv, TCM_ADJUSTRECT, (WPARAM)FALSE, (LPARAM)&rTab);
			SetWindowPos(hTab, HWND_TOP,
					rTab.left+5, rTab.top+5,
					rTab.right-rTab.left-10,
					rTab.bottom-rTab.top-10,
					SWP_NOZORDER);

			/* insert texts */
			txt = ConnectMessages(tab->msgList);
			if(NULL!=txt) SetWindowText(hTab, txt);
			free(txt);
		}
		TabCtrl_SetCurFocus(hTv,0);
		ShowWindow(hTab,SW_SHOW);
	}
}

static void OnInit(HWND hWnd, WPARAM wParam, InfDlgInitData* data)
{
	static const INT width = 400;
	static const INT height = 400;
	static const LPCTSTR iconid[] = { IDI_INFORMATION, IDI_QUESTION, IDI_WARNING, IDI_ERROR, NULL };
	static const UINT beepid[] = { MB_OK, MB_ICONQUESTION, MB_ICONEXCLAMATION, MB_ICONHAND, MB_OK };
	HWND hCtl;

	SetPieeDlgCaption(hWnd, data->caption);
	PieeCenterDialog(hWnd, width, height);

	/* Hide Cancel button */
	/*ShowWindow(GetDlgItem(hWnd,IDCANCEL), SW_HIDE);*/
	DestroyWindow(GetDlgItem(hWnd,IDCANCEL));

	/* set icon static control */
	hCtl = CreateWindowEx(
			0, _T("STATIC"),data->caption,
			WS_CHILD|SS_ICON|WS_VISIBLE,
			15, 15,
			32, 32,
			hWnd, NULL, mainInstance,
			NULL);
	if(IconType_Piee == data->icon) SendMessage(hCtl, STM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon(mainInstance, MAKEINTRESOURCE(ICON_PIEE)));
	else SendMessage(hCtl, STM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)LoadIcon(NULL, iconid[data->icon]));

	/* set description static control */
	hCtl = CreateWindowEx(
			0, _T("STATIC"),data->description,
			WS_CHILD|SS_LEFT|WS_VISIBLE,
			 60,  15,
			320,  30,
			hWnd, NULL, mainInstance,
			NULL);
	SendMessage(hCtl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE,0));

	/* set tabview control */
	if(0!=data->nTabs)
	{
		hCtl = CreateWindowEx(
				0, WC_TABCONTROL,NULL,
				WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,
				15, 70,
				370, 250,
				hWnd, (HMENU)ID_TABVIEW, mainInstance,
				NULL);
		SendMessage(hCtl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE,0));
		CreateTabs(hWnd, hCtl,data);
	}

	if(MB_OK != beepid[data->icon]) MessageBeep(beepid[data->icon]);
}

static LRESULT CommandHandler(HWND hWnd, WORD wId, WORD wNotify, HWND hControl, bool *bHandled)
{
	switch(wId)
	{
		case IDOK:
		case IDCANCEL:
			PostMessage(hWnd, WM_CLOSE, 0,0);
			return 0;
	}

	(*bHandled) = false;
	return 0;
}

static BOOL CALLBACK HideAllEdits(HWND hWnd, LPARAM lParam)
{
	ShowWindow(hWnd,(BOOL)lParam);
	return TRUE;
}

static LRESULT NotifyHandler(HWND hWnd, UINT id, LPARAM lParam, bool* bHandled)
{
	switch(id)
	{
		case ID_TABVIEW:
		{
			LPNMHDR lpnmhdr = (LPNMHDR)lParam;

			switch(lpnmhdr->code)
			{
				case TCN_SELCHANGE:
				{
					HWND hTab;
					INT inxCurSel;
					TCITEM tci;
					HWND hCtl = lpnmhdr->hwndFrom;

					tci.mask = TCIF_PARAM;
					inxCurSel = TabCtrl_GetCurSel(hCtl);
					if(TabCtrl_GetItem(hCtl,inxCurSel,&tci))
					{
						hTab = (HWND)tci.lParam;
						EnumChildWindows(hCtl, HideAllEdits, (LPARAM)SW_HIDE);
						ShowWindow(hTab, SW_SHOW);
						UpdateWindow(hTab);
					}
					return 0;
				}
			}
		}
	}
	(*bHandled) = false;
	return 0;
}

static void ReposOKButton(HWND hDlg)
{
	HWND hBtn;
	RECT rDlg, rBtn;

	hBtn = GetDlgItem(hDlg, IDOK);

	GetClientRect(hDlg,&rDlg);
	GetClientRect(hBtn,&rBtn);

	SetWindowPos(hBtn, HWND_TOP,
			rDlg.right  - (rBtn.right-rBtn.left) - 15,
			rDlg.bottom - (rBtn.bottom-rBtn.top) - 15,
			0,0,
			SWP_NOSIZE|SWP_NOZORDER);
}

INT_PTR CALLBACK InfoDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT r;
	bool bHandled = true;

	switch(msg)
	{
		case WM_INITDIALOG:
			if(NULL == (void*)lParam)
			{
				DestroyWindow(hWnd);
				return TRUE;
			}
			OnInit(hWnd, wParam, (InfDlgInitData*)lParam);
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;

		case WM_SIZE:
			ReposOKButton(hWnd);
			return TRUE;

		case WM_COMMAND:
			r = CommandHandler(hWnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam, &bHandled);
			if(bHandled) return r;
			break;

		case WM_NOTIFY:
			r = NotifyHandler(hWnd, (UINT)wParam, lParam, &bHandled);
			if(bHandled) return r;
			break;
	}
	return FALSE;
}
