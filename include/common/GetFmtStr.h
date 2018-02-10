#pragma once
/**
 * @file GetFmtStr.h
 */

#include "common/strres.h"

#if isWindows
#  include <tchar.h>
#else
   typedef char TCHAR;
#endif
TCHAR* GetFmtStr(uint);

#define putmemerr() \
	putfatal(0,GSID_MEMALLOC_FAILED,__func__)

