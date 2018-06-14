/**
 * @file MainWndProc.c
 */
#include <bolib.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include <commdlg.h>
#include <shellapi.h>
#include <commctrl.h>
#include <process.h>

#include "common/version.h"
#include "common/Observer.h"
#include "common/defines.h"
#include "common/strres.h"
#include "common/GetFmtStr.h"
#include "common/srcpath.h"
#include "common/Environment.h"
#include "common/Funex.h"
#include "win32/menures.h"
#include "win32/IniMan.h"
#include "common/MewInterfaces.h"
#include "dll/MewthreeDll.h"
#include "win32/PieeDialog.h"
#include "win32/DirButtonProc.h"
#include "win32/FileComboBoxProc.h"
#include "win32/NewListViewProc.h"
#include "win32/InfoDialogProc.h"
#include "win32/MainWndProc.h"

#define PROCESS_STATE BOOL
#define PROCESSING FALSE
#define NOT_PROCESSING TRUE

typedef enum {
	ProcMode_Install = 0,
	ProcMode_Uninstall,
} ProcMode;

typedef struct _ThreadData {
	HWND hWnd;
	MewInterface* mew;
	ProcMode mode;
	char* romPath;
	char* listPath;
	char* libsDirName;
	srcdirs dirs;
	List* defineList;
	bool isPixiCompatible;
	bool enableExtraBytes;
	bool forceReInstall;
	bool disableSscGen;
} ThreadData;
typedef struct _MewController {
	HANDLE hThread;
	ThreadData* const td;
} MewController;

static ThreadData td = {0};
static MewController mewCtrl = {NULL, &td};

extern bool vdebug;
extern bool disableWarn;
static BOOL get_DebugOption()
{
	BOOL b = IniMan_GetInstance()->get_isDebug();
	vdebug = b ? true:false;
	return b;
}
static void set_DebugOption(BOOL b)
{
	IniMan_GetInstance()->set_isDebug(b);
	vdebug = b ? true:false;
}
static BOOL get_WarnOption()
{
	BOOL b = IniMan_GetInstance()->get_isDisableWarn();
	disableWarn = b ? true:false;
	return b;
}
static void set_WarnOption(BOOL b)
{
	IniMan_GetInstance()->set_isDisableWarn(b);
	disableWarn = b ? true:false;
}

static void RegisterWndClasses(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize		= sizeof(WNDCLASSEX);

	/*--- DirButton ---*/
	GetClassInfoEx(hInstance, _T("BUTTON"), &wcex);
	lpfnDefButtonProc = wcex.lpfnWndProc;
	cbDefButtonWndExtra = wcex.cbWndExtra;
	wcex.lpfnWndProc = DirButtonProc;
	wcex.lpszClassName = DirButtonClass;
	wcex.cbWndExtra += 32;
	RegisterClassEx(&wcex);

	/*--- FileComboBox ---*/
	GetClassInfoEx(hInstance, _T("COMBOBOX"), &wcex);
	lpfnDefComboBoxProc = wcex.lpfnWndProc;
	wcex.lpfnWndProc = FileComboBoxProc;
	wcex.lpszClassName = FileComboBoxClass;
	RegisterClassEx(&wcex);

	/*--- SubItem Editable ListView ---*/
	GetClassInfoEx(hInstance, WC_LISTVIEW, &wcex);
	lpfnDefListViewProc = wcex.lpfnWndProc;
	wcex.lpfnWndProc = NewListViewProc;
	wcex.lpszClassName = NewListViewClass;
	RegisterClassEx(&wcex);
}

static void SaveWindowLocation(HWND hWnd)
{
	RECT rect;

	GetWindowRect(hWnd, &rect);
	IniMan_GetInstance()->set_X(rect.left);
	IniMan_GetInstance()->set_Y(rect.top);
}

static TCHAR** CreateRecentItems(HWND hCmb)
{
	TCHAR** items;
	INT nItems;
	INT len;
	INT i;

	nItems = SendMessage(hCmb, CB_GETCOUNT, 0,0);

	items = malloc(((size_t)nItems+1) * sizeof(TCHAR*));
	assert(items);
	for(i=nItems;0<=i;i--)
	{
		items[i] = NULL;
	}

	for(i=0;nItems>i;i++)
	{
		len = SendMessage(hCmb, CB_GETLBTEXTLEN, (WPARAM)i, 0);
		items[i] = malloc(((size_t)len+1) * sizeof(TCHAR));
		assert(items[i]);
		SendMessage(hCmb, CB_GETLBTEXT, (WPARAM)i, (LPARAM)items[i]);
	}

	return items;
}

static void SaveRecentPaths(HWND hWnd)
{
	HWND hCtl;
	TCHAR** items;
	IniMan* ini = IniMan_GetInstance();

	hCtl = GetDlgItem(hWnd, ID_CMB_ROM_FILE);
	items = CreateRecentItems(hCtl);
	ini->set_RecentRoms(items);

	hCtl = GetDlgItem(hWnd, ID_CMB_LIST_FILE);
	items = CreateRecentItems(hCtl);
	ini->set_RecentLists(items);
}

void InsertRootMenu(HMENU hMenu, HMENU hPopup, MENUITEMINFO* mii, UINT id, TCHAR* str)
{
	ZeroMemory(mii, sizeof(MENUITEMINFO));
	mii->cbSize	= sizeof(MENUITEMINFO);
	mii->fMask	= MIIM_TYPE | MIIM_ID | MIIM_SUBMENU;
	mii->fType	= MFT_STRING;
	mii->wID	= id;
	mii->hSubMenu	= hPopup;
	mii->dwTypeData	= str;
	InsertMenuItem(hMenu, id, FALSE, mii);
}

void InsertFileMenu(HMENU hMenu)
{
	HMENU hPopup;
	MENUITEMINFO mii;

	hPopup = CreateMenu();

	InsertRootMenu(hMenu, hPopup, &mii, ID_FILE, GetFmtStr(GSID_MENU_FILE));

	mii.fMask	= MIIM_TYPE | MIIM_ID;
	mii.fType	= MFT_STRING;
	mii.wID		= ID_FILE_INSERT;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_FILE_INSERT);
	InsertMenuItem(hPopup, ID_FILE_INSERT, FALSE, &mii);

	mii.wID		= ID_FILE_UNINSTALL;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_FILE_UNINSTALL);
	InsertMenuItem(hPopup, ID_FILE_UNINSTALL, FALSE, &mii);

	mii.fMask	= MIIM_TYPE | MIIM_ID | MIIM_STATE;
	mii.wID		= ID_FILE_ABORT;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_FILE_ABORT);
	mii.fState	= MFS_DISABLED;
	InsertMenuItem(hPopup, ID_FILE_ABORT, FALSE, &mii);

	mii.fMask	= MIIM_TYPE | MIIM_ID;
	mii.wID		= ID_FILE_EXIT;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_FILE_EXIT);
	InsertMenuItem(hPopup, ID_FILE_EXIT, FALSE, &mii);
}

void InsertOptionMenu(HMENU hMenu)
{
	HMENU hPopup;
	MENUITEMINFO mii;
	IniMan* ini = IniMan_GetInstance();

	hPopup = CreateMenu();

	InsertRootMenu(hMenu, hPopup, &mii, ID_OPTION, GetFmtStr(GSID_MENU_OPTION));

	mii.fMask	= MIIM_TYPE | MIIM_ID | MIIM_STATE | MIIM_DATA;
	mii.fType	= MFT_STRING;
	mii.fState	= ini->get_isPixiCompatible() ? MFS_CHECKED : MFS_UNCHECKED;
	mii.wID		= ID_OPTION_PIXI;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_OPTION_PIXI);
	mii.dwItemData	= (DWORD)ini->set_isPixiCompatible;
	InsertMenuItem(hPopup, ID_OPTION_PIXI, FALSE, &mii);

	mii.fState	= ini->get_isExtraBytes() ? MFS_CHECKED : MFS_UNCHECKED;
	mii.wID		= ID_OPTION_EBEN;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_OPTION_EBEN);
	mii.dwItemData	= (DWORD)ini->set_isExtraBytes;
	InsertMenuItem(hPopup, ID_OPTION_EBEN, FALSE, &mii);

	mii.fState	= ini->get_isForce() ? MFS_CHECKED : MFS_UNCHECKED;
	mii.wID		= ID_OPTION_FORCE;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_OPTION_FORCE);
	mii.dwItemData	= (DWORD)ini->set_isForce;
	InsertMenuItem(hPopup, ID_OPTION_FORCE, FALSE, &mii);

	mii.fState	= ini->get_disableSscGen() ? MFS_CHECKED : MFS_UNCHECKED;
	mii.wID		= ID_OPTION_SSC;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_OPTION_SSC);
	mii.dwItemData	= (DWORD)ini->set_disableSscGen;
	InsertMenuItem(hPopup, ID_OPTION_SSC, FALSE, &mii);

	mii.fMask	= MIIM_FTYPE;
	mii.fType	= MFT_SEPARATOR;
	InsertMenuItem(hPopup, 0, FALSE, &mii);

	mii.fMask	= MIIM_TYPE | MIIM_ID | MIIM_STATE | MIIM_DATA;
	mii.fType	= MFT_STRING;
	mii.fState	= get_DebugOption() ? MFS_CHECKED : MFS_UNCHECKED;
	mii.wID		= ID_OPTION_DEBUG;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_OPTION_DEBUG);
	mii.dwItemData	= (DWORD)set_DebugOption;
	InsertMenuItem(hPopup, ID_OPTION_DEBUG, FALSE, &mii);

	mii.fState	= get_WarnOption() ? MFS_CHECKED : MFS_UNCHECKED;
	mii.wID		= ID_OPTION_WARN;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_OPTION_WARN);
	mii.dwItemData	= (DWORD)set_WarnOption;
	InsertMenuItem(hPopup, ID_OPTION_WARN, FALSE, &mii);

	mii.fMask	= MIIM_FTYPE;
	mii.fType	= MFT_SEPARATOR;
	InsertMenuItem(hPopup, 1, FALSE, &mii);

	mii.fMask	= MIIM_TYPE | MIIM_ID;
	mii.fType	= MFT_STRING;
	mii.wID		= ID_OPTION_DIR;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_OPTION_DIR);
	InsertMenuItem(hPopup, ID_OPTION_DIR, FALSE, &mii);
}

void InsertHelpMenu(HMENU hMenu)
{
	HMENU hPopup;
	MENUITEMINFO mii;

	hPopup = CreateMenu();

	InsertRootMenu(hMenu, hPopup, &mii, ID_HELP, GetFmtStr(GSID_MENU_HELP));

	mii.fMask	= MIIM_TYPE | MIIM_ID;
	mii.wID		= ID_HELP_VERSION;
	mii.dwTypeData	= GetFmtStr(GSID_MENU_HELP_VERSION);
	InsertMenuItem(hPopup, ID_HELP_VERSION, FALSE, &mii);
}

static void InitMenu(HWND hWnd)
{
	HMENU hMenu;

	hMenu = CreateMenu();
	if(!hMenu) return;

	InsertFileMenu(hMenu);
	InsertOptionMenu(hMenu);
	InsertHelpMenu(hMenu);

	SetMenu(hWnd, hMenu);
}

typedef enum {
	CtrlType_Button,
	CtrlType_Static,
	CtrlType_ComboBox,
	CtrlType_FileComboBox,
	CtrlType_DirButton,
	CtrlType_ListView,
} CtrlType;
static HWND CreateControlSub(HWND hWnd, HINSTANCE hInstance, CtrlType type, HMENU id, LPCTSTR txt, UINT style, INT x, INT y, INT width, INT height, LPVOID userData)
{
	HWND hCtrl;
	switch(type)
	{
		case CtrlType_Button:
			hCtrl = CreateWindowEx(
					0, _T("BUTTON"), txt,
					WS_CHILD | WS_VISIBLE | style,
					x, y, width, height,
					hWnd,
					id,
					hInstance,
					userData);
			break;

		case CtrlType_Static:
			hCtrl = CreateWindowEx(
					0, _T("STATIC"), txt,
					WS_CHILD | WS_VISIBLE | style,
					x, y, width, height,
					hWnd,
					id,
					hInstance,
					userData);
			break;

		case CtrlType_ComboBox:
			hCtrl = CreateWindowEx(
					0, _T("COMBOBOX"), txt,
					WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | style,
					x, y, width, height,
					hWnd,
					id,
					hInstance,
					userData);
			SendMessage(hCtrl, CB_SETHORIZONTALEXTENT, (WPARAM)width, (LPARAM)0);
			SendMessage(hCtrl, CB_SETMINVISIBLE, (WPARAM)5, (LPARAM)0);
			break;

		case CtrlType_FileComboBox:
			hCtrl = CreateWindowEx(
					0, FileComboBoxClass, txt,
					WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | style,
					x, y, width, height,
					hWnd,
					id,
					hInstance,
					userData);
			SendMessage(hCtrl, CB_SETHORIZONTALEXTENT, (WPARAM)width, (LPARAM)0);
			SendMessage(hCtrl, CB_SETMINVISIBLE, (WPARAM)5, (LPARAM)0);
			break;

		case CtrlType_DirButton:
		{
			DirBtnInitData* d = (DirBtnInitData*)userData;
			ExtractIconEx(d->szSrcFileName, d->iconInx, NULL, &d->hIcon, 1);
			hCtrl = CreateWindowEx(
					0, DirButtonClass, txt,
					WS_CHILD | WS_VISIBLE | style,
					x, y, width, height,
					hWnd,
					id,
					hInstance,
					userData);
			break;
		}

		case CtrlType_ListView:
		{
			DWORD dwStyle;
			hCtrl = CreateWindowEx(
					0, NewListViewClass, txt,
					WS_CHILD | WS_VISIBLE | style,
					x, y, width, height,
					hWnd,
					id,
					hInstance,
					userData);
			dwStyle = ListView_GetExtendedListViewStyle(hCtrl);
			dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
			ListView_SetExtendedListViewStyle(hCtrl, (LPARAM)dwStyle);
			break;
		}
	}
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE,0));

	return hCtrl;
}

static void InitDefineLVColumn(HWND hLv)
{
	LV_COLUMN lvc;

	ZeroMemory(&lvc, sizeof(LV_COLUMN));

	lvc.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 160;
	lvc.pszText = GetFmtStr(GSID_LISTVIEW_COL_NAME);
	lvc.iSubItem = 0;
	ListView_InsertColumn(hLv, 0, &lvc);

	lvc.pszText = GetFmtStr(GSID_LISTVIEW_COL_VALUE);
	lvc.iSubItem = 1;
	ListView_InsertColumn(hLv, 1, &lvc);
}
static void InsertLastRow(HWND hLv)
{
	LV_ITEM lvi;
	int inx;

	ZeroMemory(&lvi, sizeof(LV_ITEM));
	inx = ListView_GetItemCount(hLv);
	lvi.mask = LVIF_TEXT;
	lvi.iItem = inx;
	lvi.iSubItem = 0;
	lvi.pszText = _T("***");
	ListView_InsertItem(hLv, &lvi);
	lvi.iSubItem = 1;
	lvi.pszText = _T("");
	ListView_SetItem(hLv, &lvi);
}
static void InitDefineLVMembers(HWND hLv, INT defInx)
{
	LV_ITEM lvi;
	int inx;
	Iterator* it;
	DefineListItem * const * const defs = IniMan_GetInstance()->get_definesLists();
	Define* d;

	if(NULL == defs) return;
	if(NULL == defs[defInx]) return;
	ZeroMemory(&lvi, sizeof(LV_ITEM));
	lvi.mask = LVIF_TEXT;

	ListView_DeleteAllItems(hLv);
	inx = 0;
	for(it=defs[defInx]->defines->begin(defs[defInx]->defines);it != NULL; it=it->next(it))
	{
		d = (Define*)it->data(it);
		lvi.iItem = inx++;
		lvi.iSubItem = 0;
		lvi.pszText = d->name;
		ListView_InsertItem(hLv, &lvi);
		lvi.iSubItem = 1;
		lvi.pszText = d->val;
		ListView_SetItem(hLv, &lvi);
	}
}

static void InitDefineCmb(HWND hCmb)
{
	uint i;
	DefineListItem * const * const defs = IniMan_GetInstance()->get_definesLists();

	if(NULL == defs) return;
	if(NULL == defs[0]) return;

	SendMessage(hCmb, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	SetWindowText(hCmb, defs[0]->name);
	for(i=0; NULL!=defs[i]; i++)
	{
		SendMessage(hCmb, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)defs[i]->name);
	}
}

static void InitRecentComboBox(HWND hCmb, TCHAR* const *  const recents)
{
	uint i;
	if(NULL==recents) return;
	if(NULL==recents[0]) return;

	SetWindowText(hCmb, recents[0]);
	for(i=0;NULL!=recents[i];i++)
	{
		SendMessage(hCmb,CB_INSERTSTRING,(WPARAM)-1,(LPARAM)recents[i]);
	}
}

static void AddRecentInfo(HWND hWnd, WORD wId, INT max)
{
	HWND hCmb;
	INT count;
	INT si;
	TCHAR buf[MAX_PATH];

	hCmb = GetDlgItem(hWnd, wId);

	GetWindowText(hCmb, buf, MAX_PATH);
	if(0 == _tcslen(buf)) return;
	si = SendMessage(hCmb,CB_FINDSTRING, (WPARAM)-1, (LPARAM)buf);
	if(CB_ERR!=si) SendMessage(hCmb,CB_DELETESTRING, (WPARAM)si, 0);
	SetWindowText(hCmb,buf);
	SendMessage(hCmb, CB_INSERTSTRING, (WPARAM)0, (LPARAM)buf);
	count = SendMessage(hCmb, CB_GETCOUNT, 0,0);

	for(;max<count;count--)
	{
		SendMessage(hCmb, CB_DELETESTRING, (WPARAM)(count-1), 0);
	}
}

static void InitControl(HWND hWnd, HINSTANCE hInstance)
{
	HWND hCtrl;
	DirBtnInitData dbid;

	RegisterWndClasses(hInstance);

	dbid.szSrcFileName = _T("shell32.dll");
	dbid.iconInx = 4; /* Folder Icon */

	/* ROM File */
	CreateControlSub(hWnd, hInstance, CtrlType_Static, NULL, GetFmtStr(GSID_MAINWND_ROM_FILE),
			SS_SIMPLE, 10, 10, 100, 20, NULL);
	hCtrl = CreateControlSub(hWnd, hInstance, CtrlType_FileComboBox, (HMENU)ID_CMB_ROM_FILE, _T(""),
			CBS_DROPDOWN, 10, 30, 300, 100, NULL);
	InitRecentComboBox(hCtrl, IniMan_GetInstance()->get_RecentRoms());
	dbid.hWnd = hCtrl;
	dbid.szFilter = GetFmtStr(GSID_OFNFILTER_ROM_FILE);
	dbid.szDefault = GetFmtStr(GSID_OFNFILTER_DEFAULT_ROM);
	CreateControlSub(hWnd, hInstance, CtrlType_DirButton, (HMENU)ID_BUTTON_DIR, NULL,
			BS_ICON, 320, 28, 40, 24, &dbid);

	/* List File */
	CreateControlSub(hWnd, hInstance, CtrlType_Static, NULL, GetFmtStr(GSID_MAINWND_LIST_FILE),
			SS_SIMPLE, 10, 60, 100, 20, NULL);
	hCtrl = CreateControlSub(hWnd, hInstance, CtrlType_FileComboBox, (HMENU)ID_CMB_LIST_FILE, _T(""),
			CBS_DROPDOWN, 10, 80, 300, 100, NULL);
	InitRecentComboBox(hCtrl, IniMan_GetInstance()->get_RecentLists());
	dbid.hWnd = hCtrl;
	dbid.szFilter = GetFmtStr(GSID_OFNFILTER_LIST_FILE);
	dbid.szDefault = GetFmtStr(GSID_OFNFILTER_DEFAULT_LIST);
	CreateControlSub(hWnd, hInstance, CtrlType_DirButton, (HMENU)ID_BUTTON_DIR, NULL,
			BS_ICON, 320, 78, 40, 24, &dbid);

	/* Defines */
	CreateControlSub(hWnd, hInstance, CtrlType_Button, NULL, GetFmtStr(GSID_MAINWND_DEFINES),
			BS_GROUPBOX, 10, 110, 360, 170, NULL);
	hCtrl = CreateControlSub(hWnd, hInstance, CtrlType_ListView, (HMENU)ID_LISTVIEW_DEFINES, _T(""),
			LVS_REPORT|LVS_EDITLABELS|LVS_OWNERDRAWFIXED, 20, 130, 340, 110, NULL);
	InitDefineLVColumn(hCtrl);
	InitDefineLVMembers(hCtrl, 0);
	InsertLastRow(hCtrl);
	hCtrl = CreateControlSub(hWnd, hInstance, CtrlType_ComboBox, (HMENU)ID_CMB_DEFINES_NAMES, _T(""),
			CBS_DROPDOWN, 20, 250, 180, 100, NULL);
	InitDefineCmb(hCtrl);
	hCtrl = CreateControlSub(hWnd, hInstance, CtrlType_Button, (HMENU)ID_BUTTON_DEFLOAD, _T("Load"),
			BS_PUSHBUTTON, 210, 250, 70, 22, NULL);
	SetWindowText(hCtrl, GetFmtStr(GSID_BUTTON_LOAD));
	hCtrl = CreateControlSub(hWnd, hInstance, CtrlType_Button, (HMENU)ID_BUTTON_DEFSAVE, _T("Save"),
			BS_PUSHBUTTON, 290, 250, 70, 22, NULL);
	SetWindowText(hCtrl, GetFmtStr(GSID_BUTTON_SAVE));

	/* Insert */
	hCtrl = CreateControlSub(hWnd, hInstance, CtrlType_Button, (HMENU)ID_FILE_INSERT, _T("Insert"),
			BS_DEFPUSHBUTTON, 160, 290, 80, 25, NULL);
	SetWindowText(hCtrl, GetFmtStr(GSID_MAINWND_INSERT));
}

static void InitStatusbar(HWND hWnd, HINSTANCE hInstance)
{
	HWND hSts;
	hSts = CreateStatusWindow(
			WS_CHILD|WS_VISIBLE|CCS_BOTTOM,
			GetFmtStr(GSID_STSBAR_READY),
			hWnd,
			ID_STSBAR);
	SendMessage(hSts, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE,0));
	CreateWindowEx(0,PROGRESS_CLASS,_T(""),
			WS_CHILD|WS_VISIBLE|PBS_SMOOTH,
			0,0,0,0,
			hSts,(HMENU)ID_PROGRESSBAR,
			hInstance, NULL);
}

static void InitWindow(HWND hWnd, HINSTANCE hInstance)
{
	INT x;
	INT y;
	static const INT Width = 400;
	static const INT Height = 400;
	UINT uFlags = SWP_HIDEWINDOW;

	InitStatusbar(hWnd, hInstance);
	x = IniMan_GetInstance()->get_X();
	y = IniMan_GetInstance()->get_Y();
	if(CW_USEDEFAULT == x || CW_USEDEFAULT == y) uFlags |= SWP_NOMOVE;
	SetWindowPos(
			hWnd, HWND_TOP,
			x, y, Width, Height,
			uFlags
		);
	InitMenu(hWnd);
	InitControl(hWnd, hInstance);
}

static void SetMenuStateSub(HWND hWnd, UINT menuId, UINT sts, MENUITEMINFO* mii)
{
	if(GetMenuItemInfo(GetMenu(hWnd), menuId, FALSE, mii))
	{
		mii->fState = sts;
		SetMenuItemInfo(GetMenu(hWnd), menuId, FALSE, mii);
	}
}

BOOL CALLBACK ChangeControlActiveSts(HWND hWnd, LPARAM lParam)
{
	BOOL sts = (BOOL)lParam;

	EnableWindow(hWnd, sts);
	return TRUE;
}

static void SetProcessingState(HWND hWnd, PROCESS_STATE sts)
{
	MENUITEMINFO mii;
	UINT stsAction = MFS_ENABLED;
	UINT stsAbort = MFS_DISABLED;

	if(PROCESSING == sts)
	{
		stsAction = MFS_DISABLED;
		stsAbort = MFS_ENABLED;
	}

	ZeroMemory(&mii, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;

	/* File Menu */
	SetMenuStateSub(hWnd, ID_FILE_INSERT, stsAction, &mii);
	SetMenuStateSub(hWnd, ID_FILE_UNINSTALL, stsAction, &mii);
	SetMenuStateSub(hWnd, ID_FILE_ABORT, stsAbort, &mii);

	/* Option Menu */
	SetMenuStateSub(hWnd, ID_OPTION, stsAction, &mii);

	/* Help Menu */
	SetMenuStateSub(hWnd, ID_HELP, stsAction, &mii);

	/* Update menu bar */
	DrawMenuBar(hWnd);

	/* Update control status */
	EnumChildWindows(hWnd, ChangeControlActiveSts, (LPARAM)sts);
}

static bool Insert(ThreadData* td)
{
	MewInterface* mew = td->mew;

	/* load list */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_PARSE_LIST);
	if(false == mew->ParseList(mew, td->listPath))
	{
		puterr(0, GSID_LIST_PARSE_FAILED);
		return false;
	}
	PostMessage(td->hWnd, WM_PIEE_SAVE_RECENT, 1,0);

	/* initialize workspace */
	putdbg(0, GSID_INSERT_CREATE_WORKSP);
	mew->InitializeWorkspace(mew, td->defineList);

	/* generate libraries data */
	putdbg(0, GSID_INSERT_COLLECT_LIBS);
	if(false == mew->CollectLibraries(mew, td->libsDirName))
	{
		return false;
	}

	/* initialize rom */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_INITIALIZE_ROM);
	if(false == mew->InitializeRom(mew, td->enableExtraBytes, td->forceReInstall, td->isPixiCompatible))
	{
		return false;
	}
	PostMessage(td->hWnd, WM_PIEE_SAVE_RECENT, 0,0);

	/* assemble all asm data */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_ASSEMBLE_ALL);
	if(false == mew->AssembleAll(mew, &td->dirs))
	{
		return false;
	}

	/* insert sprites */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_SPRITES);
	if(false == mew->InsertSprite(mew, td->disableSscGen))
	{
		return false;
	}

	/* insert extended */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_EXTENDED);
	if(false == mew->InsertExtended(mew))
	{
		return false;
	}

	/* insert cluster */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_CLUSTER);
	if(false == mew->InsertCluster(mew))
	{
		return false;
	}

	/* insert owspr */
	/*putinf(OBSFLG_IMPORTANT, GSID_INSERT_OVERWORLD);*/
	if(false == mew->InsertOverworld(mew))
	{
		return false;
	}

	putinf(0, GSID_INSERT_CHK_DATA_LEAK);
	if(false == mew->CheckDataLeak(mew))
	{
		putwar(0, GSID_INSERT_DATA_LEAK_DETECT);
		return false;
	}

	/* clean data */
	putdbg(0, GSID_INSERT_CLEAN_WORKSP);
	mew->CleanWorkspace(mew);

	return true;
}

static bool Uninstall(ThreadData* td)
{
	bool b;
	b = td->mew->UninstallAll(td->mew);
	if(b) PostMessage(td->hWnd, WM_PIEE_SAVE_RECENT, 0,0);
	return b;
}

static UINT CALLBACK ThreadProc(LPVOID lpData)
{
	bool result;
	ThreadData* td = (ThreadData*)lpData;
	bool (*proc[])(ThreadData*) = {
		Insert,
		Uninstall
	};
	static const Observers obs = {
		putdbg, /*MewObserver debug;*/
		putinf, /*MewObserver inf;*/
		putques, /*MewObserver ques;*/
		putwar, /*MewObserver warn;*/
		puterr, /*MewObserver err;*/
		putfatal, /*MewObserver fatal;*/
		progress/*ProgressNotifier_t progress;*/
	};

	td->mew = CreateMew();
	if(!td->mew) return 1;

	td->mew->RegisterObserver(td->mew, obs);
	td->mew->SetSystemPath(td->mew, Environment.CurDir, Environment.ExeDir);

	if(false == td->mew->OpenRomFile(td->mew, td->romPath))
	{
		DiscardMew(&td->mew);
		putinf(0, GSID_PROCESS_FAILED);
		PieeShowError();
		PostMessage(td->hWnd, WM_PIEE_PROCESS_END, (WPARAM)0, (LPARAM)FALSE);
		return 1;
	}

	result = proc[td->mode](td);
	if(true == result)
	{
		result = td->mew->WriteRomFile(td->mew);
		if(false == result)
		{
			putfatal(OBSFLG_FAILURE, GSID_ROM_WRITE_FAILED);
		}
	}

	td->mew->CloseRomFile(td->mew);
	DiscardMew(&td->mew);
	if(result)
	{
		putinf(0, GSID_PROCESS_SUCCEED);
	}
	else
	{
		putinf(0, GSID_PROCESS_FAILED);
	}
	PieeShowError();

	if(!result)
	{
		PostMessage(td->hWnd, WM_PIEE_PROCESS_END, (WPARAM)0, (LPARAM)FALSE);
		return 1;
	}
	PostMessage(td->hWnd, WM_PIEE_PROCESS_END, (WPARAM)0, (LPARAM)TRUE);
	return 0;
}

static void AbortProcessThread(HWND hWnd, MewController* mewCtrl)
{
	DWORD exitCode;
	assert(mewCtrl);

	if(NULL == mewCtrl->hThread) return;

	if(GetExitCodeThread(mewCtrl->hThread, &exitCode))
	{
		if(STILL_ACTIVE == exitCode)
		{
			TerminateThread(mewCtrl->hThread, 0);
			DiscardMew(&mewCtrl->td->mew);
		}
	}

	putinf(0, GSID_PROCESS_ABORTED);
	PieeShowError();
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
static List* GenerateDefinesFromLV(HWND hWnd)
{
	HWND hLv;
	List* dli;
	Define* d;
	char* dname;
	char* dval;
	int   cnt;
	int   i;
	size_t len;
	TCHAR buf[256];

	hLv = GetDlgItem(hWnd, ID_LISTVIEW_DEFINES);
	if(!hLv) return NULL;

	dli = cIniMan.CreateDefinesList();
	if(!dli) return NULL;

	cnt = (ListView_GetItemCount(hLv) - 1);

	for(i=0; i<cnt; i++)
	{
		/* name */
		ListView_GetItemText(hLv, i, 0, buf, 256);
		len = _tcslen(buf);
		RemoveSpaces(buf, &len);
		if(0==len) continue;
		dname = Str_copy(buf);

		/* value */
		ListView_GetItemText(hLv, i, 1, buf, 256);
		len = _tcslen(buf);
		RemoveSpaces(buf, &len);
		if(0==len) dval = Str_copy("1");
		else dval = Str_copy(buf);

		/* Make define */
		d = cIniMan.MakeDefine(dname, dval, false);
		dli->push(dli, d);
	}

	return dli;
}

static BOOL CreateProcessThread(HWND hWnd, MewController* mewCtrl, UINT uId)
{
	TCHAR buf[MAX_PATH];
	HWND hControl;
	UINT thid;
	/*TCHAR buf[MAX_PATH];*/
	IniMan* ini = IniMan_GetInstance();

	assert(mewCtrl);

	if(NULL != mewCtrl->hThread) return FALSE;

	mewCtrl->td->isPixiCompatible = ini->get_isPixiCompatible();
	mewCtrl->td->enableExtraBytes = ini->get_isExtraBytes();
	mewCtrl->td->forceReInstall = ini->get_isForce();
	mewCtrl->td->disableSscGen = ini->get_disableSscGen();
	mewCtrl->td->defineList = GenerateDefinesFromLV(hWnd);

	hControl = GetDlgItem(hWnd, ID_CMB_ROM_FILE);
	GetWindowText(hControl, buf, MAX_PATH);
	mewCtrl->td->romPath = Str_copy(buf);

	hControl = GetDlgItem(hWnd, ID_CMB_LIST_FILE);
	GetWindowText(hControl, buf, MAX_PATH);
	mewCtrl->td->listPath = Str_copy(buf);

	mewCtrl->td->libsDirName = ini->get_libsDir();

	mewCtrl->td->dirs.sprite = ini->get_spritesDir();
	mewCtrl->td->dirs.extended = ini->get_extendedesDir();
	mewCtrl->td->dirs.cluster = ini->get_clustersDir();

	td.hWnd = hWnd;
	mewCtrl->hThread = (HANDLE)_beginthreadex(
			NULL,
			0,
			ThreadProc,
			mewCtrl->td,
			CREATE_SUSPENDED,
			&thid);
	if(NULL==mewCtrl->hThread) return FALSE;

	return TRUE;
}

static BOOL ValidCheck(HWND hWnd)
{
	HWND hCtl;

	/* Rom file */
	hCtl = GetDlgItem(hWnd, ID_CMB_ROM_FILE);
	if(0 == GetWindowTextLength(hCtl))
	{
		puterrimm(0, GSID_INPUTERR_EMPTY, GetFmtStr(GSID_MAINWND_ROM_FILE));
		return FALSE;
	}

	/* List file */
	hCtl = GetDlgItem(hWnd, ID_CMB_LIST_FILE);
	if(0 == GetWindowTextLength(hCtl))
	{
		puterrimm(0, GSID_INPUTERR_EMPTY, GetFmtStr(GSID_MAINWND_LIST_FILE));
		return FALSE;
	}

	return TRUE;
}

static void InitInfoDlgData(InfDlgInitData* idid)
{
	TCHAR buf[2048];
	List* li = idid->tabs[0].msgList;

	idid->caption = GetFmtStr(GSID_INFDLG_VERSION);
	idid->icon = IconType_Piee;

	_stprintf_s(buf, 2048, _T("since    : December 10 2017"));
	li->push(li,Str_copy(buf));

	_stprintf_s(buf, 2048, _T("compiled : %s"), __DATE__);
	li->push(li,Str_copy(buf));

	_stprintf_s(buf, 2048, _T("           with %s ver.%s\r\n"),C_COMPILER, C_COMPILER_VER);
	li->push(li,Str_copy(buf));

	_stprintf_s(buf, 2048, _T("Mewthree : v%s"),GetMewthreeVersionStr());
	li->push(li,Str_copy(buf));

	_stprintf_s(buf, 2048, _T("Asar     : v%s API v%s"),GetAsarVersion(), GetAsarAPIVersion());
	li->push(li,Str_copy(buf));

	_stprintf_s(buf, 2048, _T("Jansson  : v%s"),GetJanssonVersion());
	li->push(li,Str_copy(buf));
}

static void LoadDefineToLV(HWND hWnd)
{
	HWND hCmb;
	HWND hLv;
	char buf[256];
	int i;
	DefineListItem * const * const defs = IniMan_GetInstance()->get_definesLists();

	hCmb = GetDlgItem(hWnd, ID_CMB_DEFINES_NAMES);
	if(!hCmb) return;

	hLv = GetDlgItem(hWnd, ID_LISTVIEW_DEFINES);
	if(!hCmb) return;

	GetWindowText(hCmb, buf, 256);

	for(i=0; NULL != defs[i]; i++)
	{
		if(NULL==defs[i]->name) continue;
		if(0 == strcmp(defs[i]->name, buf))
		{
			InitDefineLVMembers(hLv, i);
			InsertLastRow(hLv);
			return;
		}
	}
}

static char* GetDefineName(HWND hWnd, HWND hCmb)
{
	size_t len;
	TCHAR buf[256];

	GetWindowText(hCmb, buf, 256);
	len = _tcslen(buf);
	RemoveSpaces(buf, &len);
	if(0==len) return NULL;

	return Str_copy(buf);
}

static void SaveDefineInLV(HWND hWnd)
{
	List* list;
	char* name;
	DefineListItem* dli;
	HWND hCmb;

	hCmb = GetDlgItem(hWnd, ID_CMB_DEFINES_NAMES);
	if(!hCmb) return;

	name = GetDefineName(hWnd, hCmb);
	if(NULL==name) return;

	dli = malloc(sizeof(DefineListItem));
	if(NULL==dli)
	{
		free(name);
		return;
	}

	list = GenerateDefinesFromLV(hWnd);
	if(NULL==list)
	{
		free(dli);
		free(name);
		return;
	}

	dli->name = name;
	dli->defines = list;

	/* add list */
	if(IniMan_GetInstance()->add_definesList(dli))
		InitDefineCmb(hCmb);
}

/**
 * @brief Command handler
 *
 * @param hWnd Window handle
 * @param wId Command ID
 * @param wNotify Notify code
 * @param hControl Control ID
 *
 * @return TRUE: Handled / FALSE: Not handled
 */
static BOOL CommandHandler(HWND hWnd, WORD wId, WORD wNotify, HWND hControl)
{
	switch(wId)
	{
		case ID_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0,0);
			return TRUE;

		case ID_OPTION_PIXI:
		case ID_OPTION_EBEN:
		case ID_OPTION_FORCE:
		case ID_OPTION_DEBUG:
		case ID_OPTION_WARN:
		case ID_OPTION_SSC:
		{
			MENUITEMINFO mii;
			ZeroMemory(&mii, sizeof(MENUITEMINFO));
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_STATE | MIIM_DATA;
			if(GetMenuItemInfo(GetMenu(hWnd), wId, FALSE, &mii))
			{
				/* update menu */
				mii.fState = (MFS_UNCHECKED == mii.fState) ? MFS_CHECKED : MFS_UNCHECKED;
				mii.fMask = MIIM_STATE;
				SetMenuItemInfo(GetMenu(hWnd), wId, FALSE, &mii);
				DrawMenuBar(hWnd);
				/* update ini status (call "ini->set_XXX()" method) */
				((void(*)(BOOL))mii.dwItemData)((MFS_CHECKED == mii.fState) ? TRUE : FALSE);
			}
			return TRUE;
		}

		case ID_FILE_INSERT:
		case ID_FILE_UNINSTALL:
			switch(wId)
			{
				case ID_FILE_INSERT:
					mewCtrl.td->mode = ProcMode_Install;
					break;
				case ID_FILE_UNINSTALL:
					mewCtrl.td->mode = ProcMode_Uninstall;
					break;
			}
			if(!ValidCheck(hWnd)) return TRUE;
			if(!CreateProcessThread(hWnd, &mewCtrl, wId)) return TRUE;
			SetProcessingState(hWnd, PROCESSING);
			ResumeThread(mewCtrl.hThread);
			return TRUE;

		case ID_FILE_ABORT:
			AbortProcessThread(hWnd, &mewCtrl);
			PostMessage(hWnd, WM_PIEE_PROCESS_END, (WPARAM)1, (LPARAM)FALSE);
			return TRUE;

		case ID_BUTTON_DIR:
		{
			OPENFILENAME ofn;
			TCHAR szPath[MAX_PATH] = {0};
			HWND hCmb;
			LPCTSTR szFilter;

			hCmb = (HWND)GetWindowLongPtr(hControl, cbDefButtonWndExtra+0);
			szFilter = (LPCTSTR)GetWindowLongPtr(hControl, cbDefButtonWndExtra+8);

			/* get combobox text */
			GetWindowText(hCmb, szPath, MAX_PATH);
			if(0 == _tcslen(szPath))
			{
				_stprintf_s(szPath, MAX_PATH, "%s%s", Environment.ExeDir, (LPCTSTR)GetWindowLongPtr(hControl, cbDefButtonWndExtra+16));
			}

			/* FileDialog */
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = szFilter;
			ofn.lpstrFile = szPath;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST;
			if(GetOpenFileName(&ofn))
			{
				SetWindowText(hCmb, szPath);
			}
			return TRUE;
		}

		case ID_HELP_VERSION:
		{
			TabData tabs;
			InfDlgInitData idid;
			TCHAR buf[2048];

			tabs.name = GetFmtStr(GSID_INFDLG_TAB_LIBS);
			idid.nTabs = 1;
			idid.tabs = &tabs;
			tabs.msgList = new_List(NULL, free);
			InitInfoDlgData(&idid);
			_stprintf_s(buf, 2048, GetFmtStr(GSID_INFDLG_VER_DESC), Win32AppName, Win32AppVersion, CodeVersion);
			idid.description = buf;
			PieeDialog(hWnd, ID_DLG_INFO, &idid);
			delete_List(&tabs.msgList);
			return TRUE;
		}

		case ID_OPTION_DIR:
			PieeDialog(hWnd, ID_DLG_DIROPTION, NULL);
			return TRUE;

		case ID_BUTTON_DEFLOAD:
			LoadDefineToLV(hWnd);
			return TRUE;

		case ID_BUTTON_DEFSAVE:
			SaveDefineInLV(hWnd);
			return TRUE;
	}

	return FALSE;
}

static bool NotifyHandler(HWND hWnd, INT id, LPARAM lParam)
{
	switch(id)
	{
		case ID_LISTVIEW_DEFINES:
		{
			LV_DISPINFO* lvdi = (LV_DISPINFO*)lParam;
			switch(lvdi->hdr.code)
			{
				case LVN_BEGINLABELEDIT:
					/* message reflection */
					SendMessage(lvdi->hdr.hwndFrom, WM_NOTIFY, (WPARAM)id, lParam);
					return TRUE;

				case LVN_ENDLABELEDIT:
				{
					INT counts;
					/* message reflection */
					if(SendMessage(lvdi->hdr.hwndFrom, WM_NOTIFY, (WPARAM)id, lParam))
					{
						counts = ListView_GetItemCount(lvdi->hdr.hwndFrom);
						if(lvdi->item.iItem >= counts-1)
							InsertLastRow(lvdi->hdr.hwndFrom);
					}
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void SetProgress(HWND hWnd, uint cur, uint all)
{
	HWND hSts;
	HWND hProg;
	TCHAR buf[2048];

	hSts = GetDlgItem(hWnd, ID_STSBAR);
	if(!hSts) return;
	hProg = GetDlgItem(hSts, ID_PROGRESSBAR);
	if(hProg)
	{
		if(cur<all)
		{
			PostMessage(hProg, PBM_SETRANGE32, (WPARAM)0, (LPARAM)all);

			PostMessage(hProg, PBM_SETPOS, (WPARAM)cur+1, (LPARAM)0);
			PostMessage(hProg, PBM_SETPOS, (WPARAM)cur, (LPARAM)0);
		}
		else
		{
			PostMessage(hProg, PBM_SETRANGE32, (WPARAM)0, (LPARAM)all+1);
			PostMessage(hProg, PBM_SETPOS, (WPARAM)cur+1, (LPARAM)0);

			PostMessage(hProg, PBM_SETRANGE32, (WPARAM)0, (LPARAM)all);
			PostMessage(hProg, PBM_SETPOS, (WPARAM)cur, (LPARAM)0);
		}
	}
	_stprintf_s(buf, 2048, GetFmtStr(GSID_STSBAR_PROGRESS), cur, all);
	SendMessage(hSts, SB_SETTEXT, (WPARAM)0, (LPARAM)buf);
}

void ResetProgressBar(HWND hWnd)
{
	HWND hSts;
	HWND hProg;

	hSts = GetDlgItem(hWnd, ID_STSBAR);
	if(!hSts) return;
	hProg = GetDlgItem(hSts, ID_PROGRESSBAR);
	if(!hProg) return;
	SendMessage(hProg, PBM_SETPOS, (WPARAM)0, (LPARAM)0);
}

void OnSizeModified(HWND hWnd, UINT type, WORD width, WORD height)
{
	HWND hSts;
	HWND hProg;
	INT sb_size[2];
	RECT rProg;

	/* get stsbar */
	hSts = GetDlgItem(hWnd, ID_STSBAR);
	if(!hSts) return;
	hProg = GetDlgItem(hSts, ID_PROGRESSBAR);

	/* separate */
	sb_size[0] = width-120;
	sb_size[1] = width;
	SendMessage(hSts, SB_SETPARTS, 2, (LPARAM)sb_size);
	SendMessage(hSts, WM_SIZE, (WPARAM)type, MAKELPARAM(width,height));

	/* resize progressbar pos */
	SendMessage(hSts, SB_GETRECT, (LPARAM)1, (LPARAM)&rProg);
	rProg.left  += 2;
	rProg.top   += 2;
	rProg.right -= 2;
	rProg.bottom-= 2;
	SetWindowPos(hProg, HWND_TOP,
			rProg.left, rProg.top,
			rProg.right-rProg.left,
			rProg.bottom-rProg.top,
			SWP_NOZORDER);
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
			InitCommonControls();
			if(false == IniMan_GetInstance()->load())
			{
				if(MEW_OBS_YES!=putques(0,GSID_INI_LOAD_FAILED))
				{
					return -1;
				}

				/* discard ini config (for use default config) */
				IniMan_DiscardInstance();
			}
			InitWindow(hWnd, ((LPCREATESTRUCT)lParam)->hInstance);
			InitObserver(hWnd);
			return 0;

		case WM_DESTROY:
			SaveWindowLocation(hWnd);
			SaveRecentPaths(hWnd);
			if(false == IniMan_GetInstance()->save())
			{
				puterrimm(0, GSID_INI_SAVE_FAILED);
			}
			IniMan_DiscardInstance();
			UninitObserver();
			PostQuitMessage(0);
			return 0;

		case WM_NOTIFY:
			if(NotifyHandler(hWnd, (INT)wParam, lParam)) return 0;
			break;

		case WM_COMMAND:
			if(CommandHandler(hWnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam)) return 0;
			break;

		case WM_DRAWITEM:
			return SendMessage(((LPDRAWITEMSTRUCT)lParam)->hwndItem, WM_DRAWITEM, wParam, lParam);

		case WM_SIZE:
			OnSizeModified(hWnd,(UINT)wParam, LOWORD(lParam), HIWORD(lParam));
			break;

		/**
		 * WM_PIEE_PROCESS_END message
		 *   - wPatam: 0: thread termed collectly / 1: aborted
		 *   - lParam: TRUE: succeeded / FALSE: failure
		 */
		case WM_PIEE_PROCESS_END:
			/* Close thread handle */
			if(NULL != mewCtrl.hThread)
			{
				CloseHandle(mewCtrl.hThread);
				mewCtrl.hThread = NULL;
			}

			delete_List(&mewCtrl.td->defineList);
			free(mewCtrl.td->romPath);
			mewCtrl.td->romPath = NULL;
			free(mewCtrl.td->listPath);
			mewCtrl.td->listPath = NULL;

			ResetProgressBar(hWnd);

			/* mode change */
			SetProcessingState(hWnd, NOT_PROCESSING);
			return 0;

		/**
		 * WM_PIEE_SAVE_RECENT message
		 *   - wPatam: 0: ROM / 1: List
		 *   - lParam: not using
		 */
		case WM_PIEE_SAVE_RECENT:
			if(0==wParam) AddRecentInfo(hWnd, ID_CMB_ROM_FILE, IniMan_GetInstance()->get_recentRomNums());
			else AddRecentInfo(hWnd, ID_CMB_LIST_FILE, IniMan_GetInstance()->get_recentListNums());
			return 0;

		/**
		 * WM_PIEE_SETPROGRESS message
		 *   - wParam: progress current pos
		 *   - lParam: progress max pos
		 */
		case WM_PIEE_SETPROGRESS:
			SetProgress(hWnd, (uint)wParam, (uint)lParam);
			return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
