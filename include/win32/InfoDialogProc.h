#pragma once
/**
 * @file InfoDialogProc.h
 */

#define WC_INFODIALOG _T("INFODIALOG")
INT_PTR CALLBACK InfoDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef enum {
	IconType_Info = 0,
	IconType_Question,
	IconType_Warning,
	IconType_Error,
	IconType_Piee
} IconType;

typedef struct _TabData {
	TCHAR* name;
	List*  msgList;
} TabData;

typedef struct _InfDlgInitData {
	TCHAR* caption;
	TCHAR* description;
	IconType icon;
	UINT nTabs;
	TabData* tabs;
} InfDlgInitData;

#define ID_STATIC_ICON		100
#define ID_STATIC_MESSAGE	101

#define ID_TABVIEW		110
#define ID_TV_TABVERSION	111
#define ID_TV_TABDEBUG		112
#define ID_TV_TABWARNING	113
#define ID_TV_TABERROR		114
#define ID_TV_TABFATAL		115

