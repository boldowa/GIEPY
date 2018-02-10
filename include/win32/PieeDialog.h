#pragma once
/**
 * @file PieeControls.h
 */

#define GetDlgParent(hWnd) ((HWND)GetWindow(hWnd, GW_OWNER))
LRESULT PieeDialog(HWND hParent, WORD wId, LPVOID lpData);
BOOL PieeCenterDialog(HWND hDialog, INT width, INT height);
void SetPieeDlgCaption(HWND hDlg, LPCTSTR caption);

