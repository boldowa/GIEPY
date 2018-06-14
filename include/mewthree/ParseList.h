#pragma once
/**
 * @file ParseList.h
 */

#include "common/InsertList.h"

#ifdef __cplusplus
extern "C" {
#endif

/*void DiscardListData(InsertList**, Observers*);*/
bool ParseList(const char* listName, List**, MewEnvStruct*, Observers*);

#ifdef __cplusplus
} /*extern "C" */
#endif
