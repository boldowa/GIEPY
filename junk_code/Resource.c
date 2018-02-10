/**
 * @file Resource.c
 */
#include "common/types.h"
#include <windows.h>
#include <tchar.h>
#include "common/strres.h"
#include "common/Str.h"
#include "win32/Resource.h"

#define BuffLen 2048

extern HINSTANCE mainInstance;
extern uint strid_base;

TCHAR* GetResStr(HINSTANCE _hInstance, UINT id)
{
	HINSTANCE hInstance;
	TCHAR  buf[BuffLen];
	TCHAR* str = NULL;

	hInstance = _hInstance;
	if(NULL == hInstance) hInstance = mainInstance;

	if(0 == LoadString(hInstance, id+strid_base, buf, BuffLen))
	{
		if(0 == LoadString(hInstance, GSID_STR_NOT_FOUND+strid_base, buf, BuffLen))
		{
			str = Str_copy("(null)");
			return str;
		}
	}
	str = Str_copy(buf);
	return str;
}
