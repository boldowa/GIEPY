#pragma once
/**
 * @file PieeControls.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#define GetDlgParent(hWnd) ((HWND)GetWindow(hWnd, GW_OWNER))
LRESULT PieeDialog(HWND hParent, WORD wId, LPVOID lpData);
BOOL PieeCenterDialog(HWND hDialog, INT width, INT height);
void SetPieeDlgCaption(HWND hDlg, LPCTSTR caption);

#ifdef __cplusplus
} /*extern "C" */
#endif
