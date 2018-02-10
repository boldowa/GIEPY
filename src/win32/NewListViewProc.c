/**
 * @file NewListViewProc.c
 */
#include "common/types.h"
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "win32/NewLVEditProc.h"
#include "win32/NewListViewProc.h"

WNDPROC lpfnDefListViewProc = NULL;

typedef struct _ItemPos {
	INT iItem;
	INT iSubItem;
} ItemPos;

static ItemPos itemPos = { -1, -1 };
static bool bEditing = false;

static LRESULT DeleteSelectedItems(HWND hWnd)
{
	INT i;
	INT cnt;
	SendMessage(hWnd, WM_SETREDRAW, (WPARAM)FALSE, (LPARAM)0);

	cnt = ListView_GetItemCount(hWnd);
	for(i=-1;;)
	{
		i = ListView_GetNextItem(hWnd, i, LVNI_ALL|LVNI_SELECTED);
		if(0>i) break;
		if(i != (cnt-1))
		{
			ListView_DeleteItem(hWnd, i--);
		}
	}

	SendMessage(hWnd, WM_SETREDRAW, (WPARAM)TRUE, (LPARAM)0);
	return 0;
}

static LRESULT OnKeyDown(HWND hWnd, DWORD nVirtKey, DWORD lKeyData, bool* bHandled)
{
	switch(nVirtKey)
	{
		case VK_DELETE:
			(*bHandled) = true;
			return DeleteSelectedItems(hWnd);
	}
	(*bHandled) = false;
	return 0;
}

static BOOL GetListItemPos(HWND hWnd, ItemPos* itemPos)
{
	LVHITTESTINFO lvhit;
	/* Get mouse position */
	GetCursorPos(&lvhit.pt);
	ScreenToClient(hWnd, &lvhit.pt);
	ListView_SubItemHitTest(hWnd, &lvhit);
	if(0>lvhit.iItem || 0>lvhit.iSubItem || 1<lvhit.iSubItem) return FALSE;

	itemPos->iItem = lvhit.iItem;
	itemPos->iSubItem = lvhit.iSubItem;
	return TRUE;
}
static LRESULT NotifyHandler(HWND hWnd, INT id, LPARAM lParam, bool* bHandled)
{
	LV_DISPINFO* lvdi = (LV_DISPINFO*)lParam;

	if(lvdi->hdr.hwndFrom == hWnd) /* check whether reflect or not */
	{
		HWND hEdit;
		TCHAR buf[256];
		switch(lvdi->hdr.code)
		{
			case LVN_BEGINLABELEDIT:
			{
				RECT r;

				hEdit = ListView_GetEditControl(hWnd);
				if(!GetListItemPos(hWnd, &itemPos))
				{
					PostMessage(hEdit, WM_CLOSE, 0,0);
					return FALSE;
				}

				if(0 == itemPos.iSubItem)
					ListView_GetItemRect(hWnd, itemPos.iItem,&r,LVIR_LABEL);
				else
					ListView_GetSubItemRect(hWnd, itemPos.iItem, itemPos.iSubItem, LVIR_LABEL, &r);

				/* Subclass window */
				if(NULL == lpfnDefLVEditProc) lpfnDefLVEditProc = (WNDPROC)GetWindowLongPtr(hEdit, GWLP_WNDPROC);
				SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)NewLVEditProc);

				NewLVEditRect = r;
				SetWindowPos(hEdit, 0, r.left, r.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

				ListView_GetItemText(hWnd, itemPos.iItem, itemPos.iSubItem, buf, 256);
				SetWindowText(hEdit, buf);
				bEditing = true;
				(*bHandled) = true;
				return 0;
			}

			case LVN_ENDLABELEDIT:
			{
				LRESULT r = 0;
				hEdit = ListView_GetEditControl(hWnd);
				if(!GetAsyncKeyState(VK_ESCAPE) && bEditing)
				{
					bool duplicate = false;

					GetWindowText(hEdit, buf, 256);

					if(itemPos.iSubItem == 0) /* search define name */
					{
						LVFINDINFO lvfi;
						INT searchInx;

						lvfi.flags = LVFI_STRING;
						lvfi.psz = buf;
						searchInx = ListView_FindItem(hWnd, -1, &lvfi);
						if(searchInx>-1 && searchInx != itemPos.iItem)
						{
							/* define name is duplicated... */
							duplicate = true;
							/* TODO: Duplicate Error */
						}
					}
					if(!duplicate)
					{
						ListView_SetItemText(hWnd, itemPos.iItem, itemPos.iSubItem, buf);
						r = 1;
					}
				}

				/* Unsubclass window */
				SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)lpfnDefLVEditProc);

				itemPos.iItem = -1;
				itemPos.iSubItem = -1;
				bEditing = false;
				(*bHandled) = true;
				return r;
			}
		}
	}
	(*bHandled) = false;
	return 0;
}

void OnDrawItem(HWND hWnd, LPDRAWITEMSTRUCT lpDraw)
{
	HDC hdc = lpDraw->hDC;
	HBRUSH hbr;
	RECT rcItem, rcSubItem;
	RECT rcAll;
	COLORREF txtColor;
	TCHAR bufDef[256], bufVal[256];

	if(lpDraw->hwndItem != hWnd) return; /* return when it isn't reflect */

	/* Get rect */
	ListView_GetItemRect(hWnd, lpDraw->itemID, &rcAll, LVIR_BOUNDS);
	ListView_GetSubItemRect(hWnd, lpDraw->itemID, 0, LVIR_LABEL, &rcItem);
	ListView_GetSubItemRect(hWnd, lpDraw->itemID, 1, LVIR_LABEL, &rcSubItem);

	SaveDC(hdc);

	/* Set background color and font color */
	if(IsWindowEnabled(hWnd))
	{
		if((lpDraw->itemState & ODS_SELECTED) && !bEditing)
		{
			/* Active, selected */
			hbr = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
			txtColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
		}
		else
		{
			/* Active, normal */
			hbr = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
			txtColor = GetSysColor(COLOR_WINDOWTEXT);
		}
	}
	else
	{
		/* Inactive */
		hbr = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
		txtColor = GetSysColor(COLOR_GRAYTEXT);
	}

	/* set background */
	FillRect(hdc, &rcAll, hbr);

	/* Background Mode */
	SetBkMode(hdc, TRANSPARENT);

	/* Get text */
	ListView_GetItemText(hWnd, lpDraw->itemID, 0, bufDef, sizeof(TCHAR)*256);
	ListView_GetItemText(hWnd, lpDraw->itemID, 1, bufVal, sizeof(TCHAR)*256);

	/* draw text */
	SetTextColor(hdc, txtColor);
	rcSubItem.left += 4;
	if(lpDraw->itemID == itemPos.iItem)
	{
		if(0 != itemPos.iSubItem) DrawText(hdc, bufDef, -1, &rcItem, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		if(1 != itemPos.iSubItem) DrawText(hdc, bufVal, -1, &rcSubItem, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdc, bufDef, -1, &rcItem, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		DrawText(hdc, bufVal, -1, &rcSubItem, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	if((lpDraw->itemState & ODS_SELECTED) && !bEditing)
		DrawFocusRect(hdc, &rcAll);

	DeleteObject(hbr);
	RestoreDC(hdc, -1);
}

LRESULT CALLBACK NewListViewProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT r;
	bool bHandled = false;
	switch(msg)
	{
		case WM_KEYDOWN:
			r = OnKeyDown(hWnd, (DWORD)wParam, (DWORD)lParam, &bHandled);
			if(bHandled) return r;
			break;

		case WM_NOTIFY:
			r = NotifyHandler(hWnd, (INT)wParam, lParam, &bHandled);
			if(bHandled) return r;
			break;

		case WM_DRAWITEM:
			OnDrawItem(hWnd, (LPDRAWITEMSTRUCT)lParam);
			return 0;
	}
	return CallWindowProc(lpfnDefListViewProc, hWnd, msg, wParam, lParam);
}
