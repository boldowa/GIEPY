/**
 * @file GetFmtStr.c
 */

#include "common/types.h"
#if isWindows
#  include <tchar.h>
#endif
#include "common/GetFmtStr.h"
#include "common/strres.h"

#ifndef _T
#  define _T(s) (s)
#endif

uint strid_base = 0;

#define BEGIN_MESSAGE_MAP(id) switch(id) {
#define MESSAGE(id) \
	case GSID_##id: \
		 return _T(GSTR_##id);
#define END_MESSAGE_MAP() \
	default: \
		break;\
	} \
	return GetFmtStr(GSID_STR_NOT_FOUND);

TCHAR* GetFmtStr(uint id)
{
	id += strid_base;
	BEGIN_MESSAGE_MAP(id)
#include "common/msgmap_eng.h"
#include "common/msgmap_jp.h"
	END_MESSAGE_MAP()
}

