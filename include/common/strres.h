#pragma once
/**
 * @file strres.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#define		GSID_BEGIN			42000
#define		GSID_ENG_INDEX			0
#define		GSID_ENG_BEGIN			(GSID_BEGIN + GSID_ENG_INDEX)
#define		GSID_JP_INDEX			10000
#define		GSID_JP_BEGIN			(GSID_BEGIN + GSID_JP_INDEX)

/* english */
#include "strres_eng.h"
/* japanese */
#include "strres_jp.h"

#ifdef __cplusplus
} /*extern "C" */
#endif
